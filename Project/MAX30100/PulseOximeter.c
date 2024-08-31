/*
 * PulseOximeter.c
 *
 * Created: 3/8/2024 10:31:02 AM
 *  Author: Afzal Hossan
 */ 

#include "PulseOximeter.h"
#include <stdio.h> 

void PulseOximeter_init(PulseOximeter *oximeter) {
	oximeter->state = PULSEOXIMETER_STATE_INIT;
	oximeter->tsFirstBeatDetected = 0;
	oximeter->tsLastBeatDetected = 0;
	oximeter->tsLastBiasCheck = 0;
	oximeter->tsLastCurrentAdjustment = 0;
	oximeter->redLedCurrentIndex = (uint8_t)RED_LED_CURRENT_START;
	oximeter->irLedCurrent = DEFAULT_IR_LED_CURRENT;
	oximeter->onBeatDetected = NULL;
}

bool PulseOximeter_begin(PulseOximeter *oximeter, PulseOximeterDebuggingMode debuggingMode_) {
	oximeter->debuggingMode = debuggingMode_;

	bool ready = MAX30100_begin(&(oximeter->hrm));

	if (!ready) {
		return false;
	}

	MAX30100_setMode(&(oximeter->hrm), MAX30100_MODE_SPO2_HR);
	MAX30100_setLedsCurrent(&(oximeter->hrm), oximeter->irLedCurrent, (LEDCurrent)oximeter->redLedCurrentIndex);

	DCRemover_init(&(oximeter->irDCRemover), DC_REMOVER_ALPHA);
	DCRemover_init(&(oximeter->redDCRemover), DC_REMOVER_ALPHA);

	oximeter->state = PULSEOXIMETER_STATE_IDLE;

	return true;
}

void PulseOximeter_update(PulseOximeter *oximeter) {
	MAX30100_update(&(oximeter->hrm));

	PulseOximeter_checkSample(oximeter);
	PulseOximeter_checkCurrentBias(oximeter);
}

float PulseOximeter_getHeartRate(PulseOximeter *oximeter) {
	return BeatDetector_getRate(&(oximeter->beatDetector));
}

uint8_t PulseOximeter_getSpO2(PulseOximeter *oximeter) {
	return SpO2Calculator_getSpO2(&(oximeter->spO2calculator));
}

uint8_t PulseOximeter_getRedLedCurrentBias(PulseOximeter *oximeter) {
	return oximeter->redLedCurrentIndex;
}

void PulseOximeter_setOnBeatDetectedCallback(PulseOximeter *oximeter, void (*cb)()) {
	oximeter->onBeatDetected = cb;
}

void PulseOximeter_setIRLedCurrent(PulseOximeter *oximeter, LEDCurrent irLedNewCurrent) {
	oximeter->irLedCurrent = irLedNewCurrent;
	MAX30100_setLedsCurrent(&(oximeter->hrm), oximeter->irLedCurrent, (LEDCurrent)oximeter->redLedCurrentIndex);
}

void PulseOximeter_shutdown(PulseOximeter *oximeter) {
	MAX30100_shutdown(&(oximeter->hrm));
}

void PulseOximeter_resume(PulseOximeter *oximeter) {
	MAX30100_resume(&(oximeter->hrm));
}

void PulseOximeter_checkSample(PulseOximeter *oximeter) {
	uint16_t rawIRValue, rawRedValue;

	// Dequeue all available samples, they're properly timed by the HRM
	while (MAX30100_getRawValues(&(oximeter->hrm), &rawIRValue, &rawRedValue)) {
		float irACValue = DCRemover_step(&(oximeter->irDCRemover), rawIRValue);
		float redACValue = DCRemover_step(&(oximeter->redDCRemover), rawRedValue);

		// The signal fed to the beat detector is mirrored since the cleanest monotonic spike is below zero
		float filteredPulseValue = FilterBuLp1_step(&(oximeter->lpf), -irACValue);
		bool beatDetected = BeatDetector_addSample(&(oximeter->beatDetector), filteredPulseValue);

		if (BeatDetector_getRate(&(oximeter->beatDetector)) > 0) {
			oximeter->state = PULSEOXIMETER_STATE_DETECTING;
			SpO2Calculator_update(&(oximeter->spO2calculator), irACValue, redACValue, beatDetected);
			} else if (oximeter->state == PULSEOXIMETER_STATE_DETECTING) {
			oximeter->state = PULSEOXIMETER_STATE_IDLE;
			SpO2Calculator_reset(&(oximeter->spO2calculator));
		}
		if (beatDetected && oximeter->onBeatDetected) {
			oximeter->onBeatDetected();
		}
	}
}

void PulseOximeter_checkCurrentBias(PulseOximeter *oximeter) {
	// Follower that adjusts the red led current in order to have comparable DC baselines between
	// red and IR leds. The numbers are really magic: the less possible to avoid oscillations
	if (millis() - oximeter->tsLastBiasCheck > CURRENT_ADJUSTMENT_PERIOD_MS) {
		bool changed = false;
		if (DCRemover_getDCW(&(oximeter->irDCRemover)) - DCRemover_getDCW(&(oximeter->redDCRemover)) > 70000 &&
		oximeter->redLedCurrentIndex < MAX30100_LED_CURR_50MA) {
			++oximeter->redLedCurrentIndex;
			changed = true;
		} else if (DCRemover_getDCW(&(oximeter->redDCRemover)) - DCRemover_getDCW(&(oximeter->irDCRemover)) > 70000 &&
		oximeter->redLedCurrentIndex > 0) {
			--oximeter->redLedCurrentIndex;
			changed = true;
		}

		if (changed) {
			MAX30100_setLedsCurrent(&(oximeter->hrm), oximeter->irLedCurrent, (LEDCurrent)oximeter->redLedCurrentIndex);
			oximeter->tsLastCurrentAdjustment = millis();
		}

		oximeter->tsLastBiasCheck = millis();
	}
}

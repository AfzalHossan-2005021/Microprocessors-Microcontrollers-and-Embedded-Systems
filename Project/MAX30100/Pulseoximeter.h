/*
 * Pulseoximeter.h
 *
 * Created: 3/8/2024 10:26:09 AM
 *  Author: Afzal Hossan
 */ 


#ifndef PULSEOXIMETER_H_
#define PULSEOXIMETER_H_


#include <stdint.h>
#include "MAX30100.h"
#include "BeatDetector.h"
#include "Filters.h"
#include "SpO2Calculator.h"
#include "millis.h"

#define SAMPLING_FREQUENCY                  100
#define CURRENT_ADJUSTMENT_PERIOD_MS        500
#define DEFAULT_IR_LED_CURRENT              MAX30100_LED_CURR_50MA
#define RED_LED_CURRENT_START               MAX30100_LED_CURR_27_1MA
#define DC_REMOVER_ALPHA                    0.95

typedef enum {
	PULSEOXIMETER_STATE_INIT,
	PULSEOXIMETER_STATE_IDLE,
	PULSEOXIMETER_STATE_DETECTING
} PulseOximeterState;

typedef enum {
	PULSEOXIMETER_DEBUGGINGMODE_NONE,
	PULSEOXIMETER_DEBUGGINGMODE_RAW_VALUES,
	PULSEOXIMETER_DEBUGGINGMODE_AC_VALUES,
	PULSEOXIMETER_DEBUGGINGMODE_PULSEDETECT
} PulseOximeterDebuggingMode;

typedef struct {
	PulseOximeterState state;
	PulseOximeterDebuggingMode debuggingMode;
	uint32_t tsFirstBeatDetected;
	uint32_t tsLastBeatDetected;
	uint32_t tsLastBiasCheck;
	uint32_t tsLastCurrentAdjustment;
	BeatDetector beatDetector;
	DCRemover irDCRemover;
	DCRemover redDCRemover;
	FilterBuLp1 lpf;
	uint8_t redLedCurrentIndex;
	LEDCurrent irLedCurrent;
	SpO2Calculator spO2calculator;
	MAX30100 hrm;
	void (*onBeatDetected)();
} PulseOximeter;

void PulseOximeter_init(PulseOximeter *oximeter);
bool PulseOximeter_begin(PulseOximeter *oximeter, PulseOximeterDebuggingMode debuggingMode);
void PulseOximeter_update(PulseOximeter *oximeter);
float PulseOximeter_getHeartRate(PulseOximeter *oximeter);
uint8_t PulseOximeter_getSpO2(PulseOximeter *oximeter);
uint8_t PulseOximeter_getRedLedCurrentBias(PulseOximeter *oximeter);
void PulseOximeter_setOnBeatDetectedCallback(PulseOximeter *oximeter, void (*cb)());
void PulseOximeter_setIRLedCurrent(PulseOximeter *oximeter, LEDCurrent irLedCurrent);
void PulseOximeter_shutdown(PulseOximeter *oximeter);
void PulseOximeter_resume(PulseOximeter *oximeter);


#endif /* PULSEOXIMETER_H_ */
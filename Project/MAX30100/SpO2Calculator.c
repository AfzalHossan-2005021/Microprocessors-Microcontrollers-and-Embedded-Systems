/*
 * SpO2Calculator.c
 *
 * Created: 3/8/2024 10:34:54 AM
 *  Author: Afzal Hossan
 */ 


#include <math.h>
#include "SpO2Calculator.h"

const uint8_t spO2LUT[43] = {100,100,100,100,99,99,99,99,99,99,98,98,98,98,
	98,97,97,97,97,97,97,96,96,96,96,96,96,95,95,
95,95,95,95,94,94,94,94,94,93,93,93,93,93};

void SpO2Calculator_init(SpO2Calculator *calculator) {
	calculator->irACValueSqSum = 0;
	calculator->redACValueSqSum = 0;
	calculator->beatsDetectedNum = 0;
	calculator->samplesRecorded = 0;
	calculator->spO2 = 0;
}

void SpO2Calculator_update(SpO2Calculator *calculator, float irACValue, float redACValue, bool beatDetected) {
	calculator->irACValueSqSum += irACValue * irACValue;
	calculator->redACValueSqSum += redACValue * redACValue;
	++calculator->samplesRecorded;

	if (beatDetected) {
		++calculator->beatsDetectedNum;
		if (calculator->beatsDetectedNum == CALCULATE_EVERY_N_BEATS) {
			float acSqRatio = 100.0 * logf(calculator->redACValueSqSum / calculator->samplesRecorded) /
			logf(calculator->irACValueSqSum / calculator->samplesRecorded);
			uint8_t index = 0;

			if (acSqRatio > 66) {
				index = (uint8_t)acSqRatio - 66;
				} else if (acSqRatio > 50) {
				index = (uint8_t)acSqRatio - 50;
			}
			SpO2Calculator_reset(calculator);

			calculator->spO2 = spO2LUT[index];
		}
	}
}

void SpO2Calculator_reset(SpO2Calculator *calculator) {
	calculator->samplesRecorded = 0;
	calculator->redACValueSqSum = 0;
	calculator->irACValueSqSum = 0;
	calculator->beatsDetectedNum = 0;
	calculator->spO2 = 0;
}

uint8_t SpO2Calculator_getSpO2(SpO2Calculator *calculator) {
	return calculator->spO2;
}

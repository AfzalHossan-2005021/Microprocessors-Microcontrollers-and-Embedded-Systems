/*
 * SpO2Calculator.h
 *
 * Created: 3/8/2024 10:33:51 AM
 *  Author: Afzal Hossan
 */ 


#ifndef SPO2CALCULATOR_H_
#define SPO2CALCULATOR_H_


#include <stdint.h>
#include <stdbool.h>

#define CALCULATE_EVERY_N_BEATS         3

typedef struct SpO2Calculator {
	float irACValueSqSum;
	float redACValueSqSum;
	uint8_t beatsDetectedNum;
	uint32_t samplesRecorded;
	uint8_t spO2;
} SpO2Calculator;

void SpO2Calculator_init(SpO2Calculator *calculator);
void SpO2Calculator_update(SpO2Calculator *calculator, float irACValue, float redACValue, bool beatDetected);
void SpO2Calculator_reset(SpO2Calculator *calculator);
uint8_t SpO2Calculator_getSpO2(SpO2Calculator *calculator);


#endif /* SPO2CALCULATOR_H_ */
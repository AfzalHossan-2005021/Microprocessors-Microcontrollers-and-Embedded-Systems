/*
 * ADC.h
 *
 * Created: 3/7/2024 9:15:45 PM
 *  Author: Afzal Hossan
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

#define INPUT_PIN_SET_BITS 0x1F

void ADC_init();
int ADC_read(unsigned char analog_pin);


#endif /* ADC_H_ */
/*
 * ADC.c
 *
 * Created: 3/7/2024 9:17:32 PM
 *  Author: Afzal Hossan
 */ 

#include <ADC.h>

void ADC_init(){
	DDRA = 0x00;
	ADMUX = 0x00;
	ADMUX = (1 << REFS0);
	ADCSRA = 0x00;
	ADCSRA = (1 << ADEN) | (1 << ADPS2);
}

int ADC_read(unsigned char analog_pin){
	ADMUX = ADMUX | (analog_pin & INPUT_PIN_SET_BITS);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	int result = (int) ADCL;
	result = (ADCH << 8) | result;
	return result;
}

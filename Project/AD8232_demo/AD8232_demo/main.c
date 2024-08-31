/*
 * AD8232_demo.c
 *
 * Created: 2/22/2024 10:27:17 PM
 * Author : Afzal Hossan
 */ 

#include <avr/io.h>

void ADC_init(){
	ADMUX = 0x00;
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	ADCSRA = 0x00;
	ADCSRA = (1 << ADEN) | (1 << ADPS2);
}

void initialize(){
	ADC_init();
}

int main(void)
{
    initialize();
    while (1) 
    {
		
    }
}


/*
 * millis.h
 *
 * Created: 3/7/2024 10:18:21 PM
 *  Author: Afzal Hossan
 */ 


#ifndef MILLIS_H_
#define MILLIS_H_


#include <avr/io.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

volatile unsigned long timer1_millis;

ISR(TIMER1_COMPA_vect);
void init_millis(unsigned long f_cpu);
unsigned long millis (void);


#endif /* MILLIS_H_ */
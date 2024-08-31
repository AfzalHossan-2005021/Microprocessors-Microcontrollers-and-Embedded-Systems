/*
 * USART.h
 *
 * Created: 3/3/2024 9:07:13 AM
 *  Author: Afzal Hossan
 */ 


#ifndef USART_H_
#define USART_H_


#include <avr/io.h>							/* Include AVR std. library file */
#define BAUD_PRESCALE (((F_CPU / (BAUDRATE * 16UL))) - 1)	/* Define prescale value */

void USART_Init(unsigned long);				/* USART initialize function */
char USART_RxChar();						/* Data receiving function */
void USART_TxChar(char);					/* Data transmitting function */
void USART_SendString(char*);				/* Send string of USART data function */


#endif /* USART_H_ */
/*
 * I2C.h
 *
 * Created: 3/7/2024 7:55:57 PM
 *  Author: Afzal Hossan
 */ 


#ifndef I2C_H_
#define I2C_H_


#include <avr/io.h>
#include <I2C.h>
#include <inttypes.h>
#include <compat/twi.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

/* I2C clock in Hz */
#define SCL_CLOCK  8000000UL

/* I2C timer max delay */
#define I2C_TIMER_DELAY 0x00

#define TWI_STATUS_BIT 0xF8

#define BITRATE(TWSR)	((F_CPU/SCL_CLOCK)-16)/(2*pow(4,(TWSR & ((1<<TWPS0)|(1<<TWPS1)))))

#define I2C_WRITE	0
#define I2C_READ    1

void I2C_Init();
unsigned char I2C_Start(unsigned char address);
unsigned char I2C_Repeated_Start(unsigned char address);
void I2C_Start_Wait(unsigned char address);
unsigned char I2C_Write(unsigned char data);
unsigned char I2C_Read_Ack();
unsigned char I2C_Read_Nack();
unsigned char I2C_Read(unsigned char ack);
void I2C_Stop();


#endif /* I2C_H_ */
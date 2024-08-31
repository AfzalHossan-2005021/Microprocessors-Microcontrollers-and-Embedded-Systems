/*
 * MAX30102.h
 *
 * Created: 2/24/2024 2:11:57 PM
 *  Author: Afzal Hossan
 */ 


#ifndef MAX30102_H_
#define MAX30102_H_


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "i2c.h"

// Define I2C address
#define MAX30102_WRT_ADDR 0xAE
#define MAX30102_RD_ADDR 0xAF

// Other constants
#define MAX30102_FIFO_SAMPLE_STORAGE_SIZE 32


// Data structure to store sample data
typedef struct {
	uint32_t red;
	uint32_t ir;
} MAX30102_sample;

// Constants for status registers
static const uint8_t MAX30102_INT_STAT1 = 0x00;
static const uint8_t MAX30102_INT_STAT2 = 0x01;
static const uint8_t MAX30102_INT_ENABLE1 = 0x02;
static const uint8_t MAX30102_INT_ENABLE2 = 0x03;

// Constants for FIFO registers
static const uint8_t MAX30102_FIFO_WRITE_PTR = 0x04;
static const uint8_t MAX30102_FIFO_OVERFLOW = 0x05;
static const uint8_t MAX30102_FIFO_READ_PTR = 0x06;
static const uint8_t MAX30102_FIFO_DATA = 0x07;

// Configuration Registers
static const uint8_t MAX30102_FIFO_CONFIG = 0x08;
static const uint8_t MAX30102_MODE_CONFIG = 0x09;
static const uint8_t MAX30102_SPO2_CONFIG = 0x0A;
static const uint8_t MAX30102_LED1_PULSE_AMP = 0x0C;
static const uint8_t MAX30102_LED2_PULSE_AMP = 0x0D;
static const uint8_t MAX30102_MULTI_LED_CONFIG1 = 0x11;
static const uint8_t MAX30102_MULTI_LED_CONFIG2 = 0x12;

// Die Temperature Registers
static const uint8_t MAX30102_DIE_TEMP_INTGER = 0x1F;
static const uint8_t MAX30102_DIE_TEMP_FRAC = 0x20;
static const uint8_t MAX30102_DIE_TEMP_CONFIG = 0x21;

// Function implementations

uint8_t MAX30102_readRegister8(uint8_t reg)
{
	// Read 8-bit register
	uint8_t status = I2C_Start(MAX30102_WRT_ADDR);
	if (status == 1)
	{
		status = I2C_Write(reg);
		if (status == 0)
		{
			status = I2C_Repeated_Start(MAX30102_RD_ADDR);
			if (status == 1)
			{
				uint8_t data = I2C_Read_Nack();
				I2C_Stop();
				return data;
			}
		}
	}
	I2C_Stop();
	return 0;
}

void MAX30102_writeRegister8(uint8_t reg, uint8_t value)
{
	// Write 8-bit register
	uint8_t status = I2C_Start(MAX30102_WRT_ADDR);
	if (status == 1)
	{
		status = I2C_Write(reg);
		if (status == 0)
		{
			status = I2C_Write(value);
			if(status == 0)
			{
				I2C_Stop();
			}
		}
	}
}

void MAX30102_setup()
{
	// Setup sensor
	MAX30102_writeRegister8(MAX30102_MODE_CONFIG, 0x40);
	MAX30102_writeRegister8(MAX30102_INT_ENABLE1, 0x00);
	MAX30102_writeRegister8(MAX30102_INT_ENABLE2, 0x00);
	MAX30102_writeRegister8(MAX30102_FIFO_WRITE_PTR, 0);
	MAX30102_writeRegister8(MAX30102_FIFO_OVERFLOW, 0);
	MAX30102_writeRegister8(MAX30102_FIFO_READ_PTR, 0);
	MAX30102_writeRegister8(MAX30102_FIFO_CONFIG, 0x40);
	MAX30102_writeRegister8(MAX30102_MODE_CONFIG, 0x03);
	MAX30102_writeRegister8(MAX30102_SPO2_CONFIG, 0x27);
	MAX30102_writeRegister8(MAX30102_LED1_PULSE_AMP, 0x32);
	MAX30102_writeRegister8(MAX30102_LED2_PULSE_AMP, 0x32);
}

uint8_t MAX30102_available_sample_count()
{
	// Check if data is available
	int8_t writePointer = MAX30102_readRegister8(MAX30102_FIFO_WRITE_PTR);
	int8_t readPointer = MAX30102_readRegister8(MAX30102_FIFO_READ_PTR);
	if (writePointer >= readPointer)
	return (writePointer - readPointer);
	else
	return (MAX30102_FIFO_SAMPLE_STORAGE_SIZE + writePointer - readPointer);
}

MAX30102_sample MAX30102_readSample()
{
	uint8_t status = I2C_Start(MAX30102_RD_ADDR);
	if (status == 1)
	{
		status = I2C_Write(MAX30102_FIFO_DATA);
		if (status == 0)
		{
			status = I2C_Repeated_Start(MAX30102_RD_ADDR);
			if (status == 1)
			{
				MAX30102_sample sample;
				uint8_t red_23_16, red_15_8, red_7_0, ir_23_16, ir_15_8, ir_7_0;
				red_23_16 = I2C_Read_Ack();
				red_15_8 = I2C_Read_Ack();
				red_7_0 = I2C_Read_Ack();
				ir_23_16 = I2C_Read_Ack();
				ir_15_8 = I2C_Read_Ack();
				ir_7_0 = I2C_Read_Nack();
				I2C_Stop();
				sample.red = (red_23_16 << 16) | (red_15_8 << 8) | red_7_0;
				sample.ir = (ir_23_16 << 16) | (ir_15_8 << 8) | ir_7_0;
				return sample;
			}
		}
	}
}

void MAX30102_set_FIFO_RD_PTR(uint8_t value)
{
	// Set FIFO read pointer
	MAX30102_writeRegister8(MAX30102_FIFO_READ_PTR, value);
}



#endif /* MAX30102_H_ */
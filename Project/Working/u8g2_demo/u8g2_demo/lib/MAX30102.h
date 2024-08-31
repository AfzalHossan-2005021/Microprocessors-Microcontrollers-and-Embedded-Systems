/*
 * MAX30102.h
 *
 * Created: 2/24/2024 8:01:54 PM
 *  Author: Afzal Hossan
 */ 


#ifndef MAX30102_H_
#define MAX30102_H_


#include <avr-hw-i2c/i2cmaster.h>

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

uint8_t MAX30102_readRegister8(uint8_t reg){
	unsigned char status = i2c_start(MAX30102_WRT_ADDR);
	if(status){
		status = i2c_write(reg);
		if(status){
			status = i2c_rep_start(MAX30102_RD_ADDR);
			if(status){
				uint8_t data = i2c_readNak();
				i2c_stop();
				return data;
			}
		}
	}
	return 0;
}

void MAX30102_writeRegister8(uint8_t reg, uint8_t value){
	unsigned char status = i2c_start(MAX30102_WRT_ADDR);
	if(status){
		status = i2c_write(reg);
		if(status){
			status = i2c_write(value);
			if(status){
				i2c_stop();
			}
		}
	}
}

void MAX30102_setup(){
	// Setup sensor
	i2c_init();
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

uint8_t read_WRITE_PTR(){
	return MAX30102_readRegister8(MAX30102_FIFO_WRITE_PTR);
}


#endif /* MAX30102_H_ */
/*
 * MAX30100.h
 *
 * Created: 3/8/2024 10:44:08 AM
 *  Author: Afzal Hossan
 */ 


#ifndef MAX30100_H_
#define MAX30100_H_


#include <stdint.h>
#include <stdbool.h>
#include "CircularBuffer.h"
#include "Registers.h"
#include "I2C.h"

#define DEFAULT_MODE                MAX30100_MODE_HRONLY
#define DEFAULT_SAMPLING_RATE       MAX30100_SAMPRATE_100HZ
#define DEFAULT_PULSE_WIDTH         MAX30100_SPC_PW_1600US_16BITS
#define DEFAULT_RED_LED_CURRENT     MAX30100_LED_CURR_50MA
#define DEFAULT_IR_LED_CURRENT      MAX30100_LED_CURR_50MA
#define EXPECTED_PART_ID            0x11
#define RINGBUFFER_SIZE             16

#define WRITE_ADDRESS				0xAE
#define READ_ADDRESS				0xAF

typedef struct {
	CircularBuffer *readoutsBuffer;
} MAX30100;

bool MAX30100_begin(MAX30100 *self);
void MAX30100_setMode(MAX30100 *self, Mode mode);
void MAX30100_setLedsPulseWidth(MAX30100 *self, LEDPulseWidth ledPulseWidth);
void MAX30100_setSamplingRate(MAX30100 *self, SamplingRate samplingRate);
void MAX30100_setLedsCurrent(MAX30100 *self, LEDCurrent irLedCurrent, LEDCurrent redLedCurrent);
void MAX30100_setHighresModeEnabled(MAX30100 *self, bool enabled);
void MAX30100_update(MAX30100 *self);
bool MAX30100_getRawValues(MAX30100 *self, uint16_t *ir, uint16_t *red);
void MAX30100_resetFifo(MAX30100 *self);
void MAX30100_startTemperatureSampling(MAX30100 *self);
bool MAX30100_isTemperatureReady(MAX30100 *self);
float MAX30100_retrieveTemperature(MAX30100 *self);
void MAX30100_shutdown(MAX30100 *self);
void MAX30100_resume(MAX30100 *self);
uint8_t MAX30100_getPartId(MAX30100 *self);

uint8_t readRegister(uint8_t address);
void writeRegister(uint8_t address, uint8_t data);
void burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length);
void readFifoData(MAX30100 *self);


#endif /* MAX30100_H_ */
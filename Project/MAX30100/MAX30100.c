/*
 * MAX30100.c
 *
 * Created: 3/8/2024 10:43:50 AM
 *  Author: Afzal Hossan
 */ 

#include "MAX30100.h"
#include "I2C.h"

#define WRITE_ADDRESS				0xAE
#define READ_ADDRESS				0xAF

bool MAX30100_begin(MAX30100 *self)
{
	if (MAX30100_getPartId(self) != EXPECTED_PART_ID) {
		return false;
	}

	MAX30100_setMode(self, DEFAULT_MODE);
	MAX30100_setLedsPulseWidth(self, DEFAULT_PULSE_WIDTH);
	MAX30100_setSamplingRate(self, DEFAULT_SAMPLING_RATE);
	MAX30100_setLedsCurrent(self, DEFAULT_IR_LED_CURRENT, DEFAULT_RED_LED_CURRENT);
	MAX30100_setHighresModeEnabled(self, true);

	return true;
}

void MAX30100_setMode(MAX30100 *self, Mode mode)
{
	writeRegister(MAX30100_REG_MODE_CONFIGURATION, mode);
}

void MAX30100_setLedsPulseWidth(MAX30100 *self, LEDPulseWidth ledPulseWidth)
{
	uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
	writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xfc) | ledPulseWidth);
}

void MAX30100_setSamplingRate(MAX30100 *self, SamplingRate samplingRate)
{
	uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
	writeRegister(MAX30100_REG_SPO2_CONFIGURATION, (previous & 0xe3) | (samplingRate << 2));
}

void MAX30100_setLedsCurrent(MAX30100 *self, LEDCurrent irLedCurrent, LEDCurrent redLedCurrent)
{
	writeRegister(MAX30100_REG_LED_CONFIGURATION, redLedCurrent << 4 | irLedCurrent);
}

void MAX30100_setHighresModeEnabled(MAX30100 *self, bool enabled)
{
	uint8_t previous = readRegister(MAX30100_REG_SPO2_CONFIGURATION);
	if (enabled) {
		writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous | MAX30100_SPC_SPO2_HI_RES_EN);
		} else {
		writeRegister(MAX30100_REG_SPO2_CONFIGURATION, previous & ~MAX30100_SPC_SPO2_HI_RES_EN);
	}
}

void MAX30100_update(MAX30100 *self)
{
	readFifoData(self);
}

bool MAX30100_getRawValues(MAX30100 *self, uint16_t *ir, uint16_t *red)
{
	if (!CircularBuffer_IsEmpty(self->readoutsBuffer)) {
		SensorReadout readout = CircularBuffer_Pop(self->readoutsBuffer);

		*ir = readout.ir;
		*red = readout.red;

		(self->readoutsBuffer->tail) = ((self->readoutsBuffer->tail) + 1) % MAX30100_FIFO_DEPTH;

		return true;
	}
	return false;
}

void MAX30100_resetFifo(MAX30100 *self)
{
	writeRegister(MAX30100_REG_FIFO_WRITE_POINTER, 0);
	writeRegister(MAX30100_REG_FIFO_READ_POINTER, 0);
	writeRegister(MAX30100_REG_FIFO_OVERFLOW_COUNTER, 0);
}

void MAX30100_startTemperatureSampling(MAX30100 *self)
{
	uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
	modeConfig |= MAX30100_MC_TEMP_EN;

	writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

bool MAX30100_isTemperatureReady(MAX30100 *self)
{
	return !(readRegister(MAX30100_REG_MODE_CONFIGURATION) & MAX30100_MC_TEMP_EN);
}

float MAX30100_retrieveTemperature(MAX30100 *self)
{
	int8_t tempInteger = readRegister(MAX30100_REG_TEMPERATURE_DATA_INT);
	float tempFrac = readRegister(MAX30100_REG_TEMPERATURE_DATA_FRAC);

	return tempFrac * 0.0625 + tempInteger;
}

void MAX30100_shutdown(MAX30100 *self)
{
	uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
	modeConfig |= MAX30100_MC_SHDN;

	writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

void MAX30100_resume(MAX30100 *self)
{
	uint8_t modeConfig = readRegister(MAX30100_REG_MODE_CONFIGURATION);
	modeConfig &= ~MAX30100_MC_SHDN;

	writeRegister(MAX30100_REG_MODE_CONFIGURATION, modeConfig);
}

uint8_t MAX30100_getPartId(MAX30100 *self)
{
	return readRegister(0xff);
}

uint8_t readRegister(uint8_t address)
{
	I2C_Start(WRITE_ADDRESS);
	I2C_Write(address);
	I2C_Start(READ_ADDRESS);
	uint8_t data = I2C_Read_Nack();
	I2C_Stop();

	return data;
}

void writeRegister(uint8_t address, uint8_t data)
{
	I2C_Start(WRITE_ADDRESS);
	I2C_Write(address);
	I2C_Write(data);
	I2C_Stop();
}

void burstRead(uint8_t baseAddress, uint8_t *buffer, uint8_t length)
{
	I2C_Start(WRITE_ADDRESS);
	I2C_Write(baseAddress);
	I2C_Repeated_Start(READ_ADDRESS);

	for (uint8_t i = 0; i < length - 1; i++) {
		buffer[i] = I2C_Read_Ack();
	}
	buffer[length - 1] = I2C_Read_Nack();

	I2C_Stop();
}

void readFifoData(MAX30100 *self) {
	uint8_t buffer[MAX30100_FIFO_DEPTH * 4];
	uint8_t toRead;

	toRead = (readRegister(MAX30100_REG_FIFO_WRITE_POINTER) - readRegister(MAX30100_REG_FIFO_READ_POINTER)) & (MAX30100_FIFO_DEPTH - 1);

	if (toRead) {
		burstRead(MAX30100_REG_FIFO_DATA, buffer, 4 * toRead);

		for (uint8_t i = 0; i < toRead; ++i) {
			self->readoutsBuffer->buffer[(*(self->readoutsBuffer->head) + i) % MAX30100_FIFO_DEPTH] = (SensorReadout){
				.ir = (uint16_t)((buffer[i * 4] << 8) | buffer[i * 4 + 1]),
			.red = (uint16_t)((buffer[i * 4 + 2] << 8) | buffer[i * 4 + 3])};
		}

		*(self->readoutsBuffer->head) = (*(self->readoutsBuffer->head) + toRead) % MAX30100_FIFO_DEPTH;
	}
}

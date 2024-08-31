/*
 * u8x8_Wrapper.c
 *
 * Created: 3/7/2024 8:56:40 PM
 *  Author: Afzal Hossan
 */ 

#include <util/delay.h>
#include <u8x8_Wrapper.h>

#ifdef AVR_USE_HW_I2C
#include <I2C.h>

uint8_t u8x8_byte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
	uint8_t *data;
	switch(msg){
		case U8X8_MSG_BYTE_SEND:
			data = (uint8_t *)arg_ptr;
			while( arg_int-- )
				I2C_Write(*data++);
			break;
		case U8X8_MSG_BYTE_INIT:
			break;
		case U8X8_MSG_BYTE_SET_DC:
			break;
		case U8X8_MSG_BYTE_START_TRANSFER:
			I2C_Start_Wait(u8x8_GetI2CAddress(u8x8)+I2C_WRITE);
			break;
		case U8X8_MSG_BYTE_END_TRANSFER:
			I2C_Stop();
			break;
		default:
			return 0;
	}
	return 1;
}
#endif

uint8_t u8x8_delay (u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
	switch(msg) {
		case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
			while( arg_int-- ) _delay_us(10);
			break;
		case U8X8_MSG_DELAY_MILLI:  // delay arg_int * 1 milli second
			while( arg_int-- ) _delay_ms(1);
			break;
		default:
			return 0;
	}
	return 0;
}
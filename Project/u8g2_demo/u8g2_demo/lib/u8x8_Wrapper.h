/*
 * u8x8_Wrapper.h
 *
 * Created: 3/7/2024 8:54:08 PM
 *  Author: Afzal Hossan
 */ 


#ifndef U8X8_WRAPPER_H_
#define U8X8_WRAPPER_H_


#include <u8g2.h>
#include <stdint.h>

uint8_t u8x8_byte(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);
uint8_t u8x8_delay(u8x8_t * u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr);


#endif /* U8X8_WRAPPER_H_ */
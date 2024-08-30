/*
 * u8g2_demo.c
 *
 * Created: 2/21/2024 8:02:58 PM
 * Author : Afzal Hossan
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <u8g2.h>
#include <u8x8_Wrapper.h>
#include <inttypes.h>
#include <string.h>
#include <USART.h>
#include <I2C.h>
#include <ADC.h>

#define OLED_ADDR  0x78

u8g2_t u8g2;
char buffer[32]; // helper buffer to construct a string to be displayed
unsigned char last_y = 0;

const unsigned char OXYMETER_PROMPT [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfd, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x1f, 0xfe, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xfc, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x01, 0xf0, 0xff, 0x01, 0xe0, 0xff, 0xff, 0xff, 0xff, 0x88, 0xfc, 0xff, 0xff, 0xff, 0xff,
	0x7f, 0x00, 0xc0, 0xff, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0x9c, 0xfc, 0xff, 0xff, 0xff, 0xff,
	0x1f, 0x00, 0x80, 0x3f, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0x9c, 0x18, 0x3e, 0x0e, 0xff, 0xff,
	0x0f, 0x00, 0x00, 0x1e, 0x00, 0x00, 0xfe, 0xff, 0xff, 0xff, 0x88, 0x1c, 0x0c, 0x06, 0xfe, 0xff,
	0x07, 0xe0, 0x00, 0x0c, 0xe0, 0x01, 0xfc, 0xff, 0xff, 0xff, 0xc0, 0x3c, 0xcc, 0x07, 0xfe, 0xff,
	0x03, 0xfc, 0x07, 0x00, 0xfc, 0x0f, 0xf8, 0xff, 0xff, 0xff, 0xf8, 0x1c, 0xcc, 0x07, 0xfe, 0xff,
	0x03, 0xff, 0x0f, 0x00, 0xfe, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xfc, 0xcc, 0xcc, 0xe7, 0xff, 0xff,
	0x81, 0xff, 0x3f, 0x00, 0xff, 0x3f, 0xf0, 0xff, 0xff, 0xff, 0xfc, 0x0c, 0x8c, 0xc6, 0xfe, 0xff,
	0xc1, 0xff, 0x7f, 0x80, 0xff, 0x7f, 0xf0, 0xff, 0xff, 0xff, 0xfc, 0x1c, 0x1c, 0x0e, 0xfe, 0xff,
	0xc1, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xff, 0xff, 0xfb, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xff, 0xff, 0xff, 0x03, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xff, 0xff, 0xff, 0x00, 0xfc, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xff, 0xff, 0x3f, 0x00, 0xf0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xe0, 0xff, 0xff, 0x1f, 0x00, 0xe0, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xc1, 0xff, 0xff, 0x1f, 0x00, 0xe0, 0xe0, 0xff, 0xff, 0xff, 0xf3, 0xfc, 0xff, 0xff, 0xff, 0xff,
	0xc1, 0xff, 0xff, 0x0f, 0x00, 0x40, 0x80, 0xff, 0xff, 0xff, 0x63, 0xfe, 0xff, 0xff, 0xff, 0xff,
	0xc1, 0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0xfc, 0xff, 0xff, 0x67, 0xfe, 0xff, 0xff, 0xff, 0xff,
	0x83, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x07, 0x87, 0x77, 0xb6, 0xff, 0xff,
	0x03, 0xff, 0xff, 0x07, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0x0f, 0x03, 0x73, 0x86, 0xff, 0xff,
	0x07, 0xfe, 0xff, 0x07, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0x8f, 0x31, 0x72, 0xe6, 0xff, 0xff,
	0x07, 0xfe, 0xff, 0x07, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x9f, 0x79, 0x72, 0xe6, 0xff, 0xff,
	0x0f, 0xfc, 0xff, 0x07, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0x9f, 0x79, 0x72, 0xe6, 0xff, 0xff,
	0x1f, 0xf8, 0xff, 0x07, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x9f, 0x33, 0x23, 0xe6, 0xff, 0xff,
	0x3f, 0xf0, 0xff, 0x07, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x9f, 0x83, 0x07, 0xe6, 0xff, 0xff,
	0x3f, 0xc0, 0xff, 0x07, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0x7f, 0x80, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x00, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x01, 0xfe, 0x1f, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x07, 0xfc, 0x1f, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x0f, 0xf8, 0x3f, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x1f, 0xe0, 0x3f, 0x00, 0x00, 0x00, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x3f, 0xc0, 0x7f, 0x00, 0x00, 0x00, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x7f, 0x80, 0x7f, 0x00, 0x00, 0x00, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x01, 0x1f, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x03, 0x0e, 0x00, 0x00, 0x00, 0xfc, 0xff, 0x1f, 0xc8, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x07, 0x0c, 0x00, 0x00, 0x00, 0xfe, 0xff, 0x1f, 0xce, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0x00, 0x00, 0x00, 0x80, 0xff, 0xff, 0x9f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x1f, 0x00, 0x03, 0x00, 0xe0, 0xff, 0xff, 0x9f, 0xdf, 0xc4, 0x83, 0xe3, 0xe5, 0xff,
	0xff, 0xff, 0x3f, 0x80, 0x07, 0x00, 0xf8, 0xff, 0xff, 0x1f, 0xcf, 0x80, 0x81, 0xc1, 0xe0, 0xff,
	0xff, 0xff, 0x7f, 0xc0, 0x07, 0x00, 0xff, 0xff, 0xff, 0x1f, 0xc8, 0x98, 0xc9, 0x80, 0xf8, 0xff,
	0xff, 0xff, 0xff, 0xe0, 0x0f, 0xc0, 0xff, 0xff, 0xff, 0x1f, 0xcf, 0x9c, 0xc1, 0x80, 0xfc, 0xff,
	0xff, 0xff, 0xff, 0xf1, 0x0f, 0xf8, 0xff, 0xff, 0xff, 0x9f, 0xcf, 0x9c, 0xe1, 0xfc, 0xf8, 0xff,
	0xff, 0xff, 0xff, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0xff, 0x9f, 0xcf, 0x9c, 0xf9, 0xd8, 0xfc, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xbf, 0xff, 0xff, 0xff, 0xff, 0x9f, 0xcf, 0x9c, 0xc1, 0xc1, 0xfc, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x98, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x9c, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const unsigned char ECG_PROMPT [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x00, 0xfc, 0xff, 0x07, 0xc0, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x0f, 0x00, 0xe0, 0xff, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0x07, 0xfe, 0xc0, 0x3f, 0xe0, 0x0f, 0xfc, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xc3, 0xff, 0x07, 0x1f, 0xfc, 0x7f, 0xf8, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xe0, 0xff, 0x1f, 0x0e, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xf8, 0xff, 0x3f, 0x84, 0xff, 0xff, 0xe1, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x1f, 0xff, 0xff, 0x7f, 0xfc, 0xff, 0x7f, 0xc0, 0xff, 0xff, 0xc3, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0xfe, 0xff, 0x3f, 0xfc, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xc7, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0xfe, 0xff, 0x3f, 0xfe, 0xff, 0xff, 0xf1, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0xfe, 0xff, 0x3f, 0xfe, 0xff, 0xff, 0xf9, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0xfe, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0xfe, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0xfe, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0xfe, 0xff, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x0f, 0xfe, 0xff, 0x1f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0xff, 0xff, 0xff,
	0xff, 0xff, 0x07, 0xfe, 0x9f, 0x3f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x3f, 0xff, 0xff,
	0xff, 0xff, 0x07, 0xfe, 0x8f, 0x3f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x8f, 0x3f, 0xfe, 0xff,
	0xff, 0xff, 0x07, 0xfe, 0x0f, 0x3f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0x87, 0x1f, 0xfe, 0xff,
	0xff, 0xff, 0x07, 0xfc, 0x0f, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc7, 0x1f, 0xfe, 0xff,
	0xff, 0xff, 0x47, 0xfc, 0x0f, 0x7f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xc3, 0x1f, 0xfe, 0xff,
	0xff, 0xff, 0x47, 0xfc, 0x0f, 0xff, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe3, 0x1f, 0xfe, 0xff,
	0xff, 0xff, 0x47, 0xfc, 0x07, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0x1f, 0xfc, 0xff,
	0xff, 0xff, 0x47, 0xfc, 0x07, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x1f, 0xfc, 0xff,
	0xff, 0xff, 0x47, 0xfc, 0x07, 0xfe, 0xe3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf8, 0x1f, 0xfc, 0xff,
	0xff, 0xff, 0x63, 0x8c, 0x07, 0xfe, 0xc3, 0xff, 0xff, 0xff, 0xff, 0x7f, 0xfc, 0x0f, 0xfc, 0xff,
	0xff, 0xff, 0x63, 0x8c, 0x07, 0xfe, 0x87, 0xff, 0xff, 0xff, 0xff, 0x3f, 0xfc, 0x0f, 0xfc, 0xff,
	0xff, 0xff, 0x63, 0x84, 0x07, 0xfe, 0x0f, 0xff, 0xff, 0xff, 0xff, 0x1f, 0xfe, 0x0f, 0xf8, 0xff,
	0xff, 0xff, 0x63, 0x00, 0x23, 0xfe, 0x1f, 0xfe, 0xff, 0xff, 0xff, 0x0f, 0xff, 0x8f, 0xf8, 0xff,
	0xff, 0xff, 0xe3, 0x00, 0x23, 0xbe, 0x3f, 0xfc, 0xff, 0xff, 0xff, 0x87, 0xbf, 0x8f, 0xf8, 0xff,
	0xff, 0xcf, 0xe3, 0x00, 0x22, 0x1e, 0x7f, 0xf8, 0xff, 0xff, 0xff, 0xc3, 0x1f, 0x8f, 0xf8, 0xff,
	0xff, 0xc7, 0xe3, 0x20, 0x62, 0x1c, 0xfe, 0xf0, 0xff, 0xff, 0xff, 0xe1, 0x0f, 0xc7, 0xf8, 0xff,
	0xff, 0x87, 0xe3, 0x20, 0x60, 0x0c, 0xfe, 0xe1, 0xff, 0xff, 0xff, 0xf0, 0x0f, 0xc6, 0xf8, 0xff,
	0xff, 0x83, 0xe3, 0x70, 0x70, 0x0c, 0xfc, 0xc3, 0xff, 0xff, 0x3f, 0xf8, 0x07, 0xc6, 0xf1, 0xff,
	0xff, 0x83, 0xf1, 0x70, 0x70, 0x0c, 0xfc, 0x07, 0xff, 0xff, 0x1f, 0xfe, 0x07, 0xc6, 0xf1, 0xff,
	0xff, 0x81, 0xf1, 0xf0, 0x70, 0xcc, 0xf8, 0x1f, 0xfe, 0xff, 0x0f, 0xff, 0x43, 0xc4, 0xf1, 0xff,
	0xff, 0x80, 0xf1, 0xf8, 0x70, 0xc4, 0xf8, 0x1f, 0xfc, 0xff, 0x07, 0xff, 0x63, 0xc4, 0xf1, 0xff,
	0x00, 0x88, 0xf1, 0xf9, 0x70, 0xc0, 0x00, 0x00, 0xf8, 0xff, 0x03, 0x00, 0x60, 0xc0, 0x01, 0x00,
	0x00, 0x88, 0xf1, 0xff, 0xf9, 0xc0, 0x01, 0x00, 0xf8, 0xff, 0x03, 0x00, 0x70, 0xe0, 0x03, 0x00,
	0xff, 0x8f, 0xf1, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff,
	0xff, 0x8f, 0xf1, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff,
	0xff, 0x8f, 0xf1, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff,
	0xff, 0x9f, 0xf1, 0xff, 0xff, 0xe0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0xff, 0xff,
	0xff, 0x1f, 0xf9, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xff, 0xff,
	0xff, 0x1f, 0xf8, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe1, 0xff, 0xff,
	0xff, 0x1f, 0xf8, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff,
	0xff, 0x1f, 0xf8, 0xff, 0xff, 0xf9, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf3, 0xff, 0xff,
	0xff, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x1f, 0xf8, 0xff, 0xff, 0xff, 0xfe, 0xfd, 0xff, 0xef, 0xff, 0xff, 0xe7, 0xff, 0xef, 0xff,
	0xff, 0x3f, 0xf8, 0xbc, 0xf9, 0xff, 0xfc, 0xf9, 0xff, 0xe7, 0xbf, 0xff, 0xe7, 0xff, 0xef, 0xff,
	0xff, 0x3f, 0xfc, 0x18, 0xf9, 0xff, 0xfc, 0xf9, 0xff, 0xe7, 0x3f, 0xff, 0xe7, 0xff, 0xe7, 0xff,
	0xff, 0x3f, 0x7c, 0x1a, 0x79, 0xe6, 0xe4, 0x49, 0xdf, 0x64, 0x3f, 0x97, 0x07, 0xd3, 0x24, 0xfb,
	0xff, 0x3f, 0x7c, 0xbb, 0xf9, 0xf6, 0xe4, 0x69, 0x9e, 0x26, 0x3f, 0xbb, 0x67, 0x99, 0x66, 0xf9,
	0xff, 0x3f, 0x7c, 0xb3, 0x19, 0xf6, 0xe4, 0x09, 0x86, 0x66, 0x3c, 0xbb, 0x67, 0x99, 0x6e, 0xfd,
	0xff, 0x3f, 0x3c, 0xb2, 0xd9, 0xf6, 0xe4, 0xc9, 0x9b, 0xe6, 0x3d, 0xbb, 0x67, 0x9b, 0x64, 0xfc,
	0xff, 0x3f, 0xbc, 0x27, 0x13, 0xce, 0xe4, 0x19, 0x83, 0x60, 0x7c, 0xc6, 0x07, 0xc3, 0xe0, 0xfc,
	0xff, 0x3f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc,
	0xff, 0x3f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe,
	0xff, 0x3f, 0xfc, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x7f, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

const unsigned char QR_CODE [] PROGMEM = {
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0x00, 0xe0, 0x1f, 0xf8, 0x07, 0x00, 0xff, 0xff, 0x00, 0xe0, 0x1f, 0xf8, 0x07, 0x00, 0xff,
	0xff, 0xfc, 0xe7, 0xe1, 0xff, 0xe7, 0x3f, 0xff, 0xff, 0xfc, 0xe7, 0xe1, 0xff, 0xe7, 0x3f, 0xff,
	0xff, 0x0c, 0xe6, 0x7f, 0x78, 0x66, 0x30, 0xff, 0xff, 0x0c, 0xe6, 0x7f, 0x78, 0x67, 0x30, 0xff,
	0xff, 0x0c, 0x66, 0x7e, 0x86, 0x67, 0x30, 0xff, 0xff, 0x0c, 0x66, 0x7c, 0x86, 0x67, 0x30, 0xff,
	0xff, 0x0c, 0x66, 0xf8, 0x99, 0x67, 0x30, 0xff, 0xff, 0xfc, 0xe7, 0x1f, 0x80, 0xe7, 0x3f, 0xff,
	0xff, 0xfc, 0xe7, 0x1f, 0x80, 0xe7, 0x3f, 0xff, 0xff, 0x00, 0xe0, 0x66, 0x66, 0x07, 0x00, 0xff,
	0xff, 0x00, 0x60, 0x66, 0x66, 0x06, 0x00, 0xff, 0xff, 0xff, 0xff, 0x87, 0x61, 0xfe, 0xff, 0xff,
	0xff, 0xff, 0xff, 0x87, 0x61, 0xfe, 0xff, 0xff, 0xff, 0xf8, 0x83, 0x99, 0x19, 0x7e, 0xfc, 0xff,
	0xff, 0xf0, 0x81, 0x99, 0x19, 0x7e, 0xfc, 0xff, 0xff, 0xe0, 0x91, 0x79, 0x66, 0x18, 0x02, 0xff,
	0xff, 0xc0, 0x99, 0x79, 0x66, 0x18, 0x03, 0xff, 0xff, 0x3c, 0x60, 0xe6, 0x1f, 0x20, 0x33, 0xff,
	0xff, 0x3c, 0x60, 0xe6, 0x1f, 0x60, 0x33, 0xff, 0xff, 0x4c, 0xfe, 0x79, 0x86, 0x19, 0xff, 0xff,
	0xff, 0xcc, 0xfe, 0x79, 0x86, 0x19, 0xff, 0xff, 0xff, 0x00, 0xe7, 0x19, 0x80, 0x9f, 0xc3, 0xff,
	0xff, 0x00, 0xe7, 0x19, 0x80, 0x9f, 0xc3, 0xff, 0xff, 0x0c, 0xfe, 0xf9, 0x1f, 0xe6, 0xf3, 0xff,
	0xff, 0x0c, 0xfe, 0xf9, 0x1f, 0xe6, 0xf3, 0xff, 0xff, 0xfc, 0x01, 0x00, 0x7e, 0xe6, 0x3f, 0xff,
	0xff, 0xfc, 0x01, 0x00, 0x7e, 0xe6, 0x3f, 0xff, 0xff, 0x0c, 0x78, 0x9e, 0x07, 0x60, 0x33, 0xff,
	0xff, 0x0c, 0x70, 0x9e, 0x07, 0x20, 0x32, 0xff, 0xff, 0x0c, 0xe1, 0x87, 0x1f, 0x00, 0x30, 0xff,
	0xff, 0x8c, 0xe3, 0x87, 0x1f, 0x00, 0x20, 0xff, 0xff, 0xff, 0x7f, 0x60, 0x18, 0x7e, 0x03, 0xff,
	0xff, 0xff, 0x7f, 0x60, 0x18, 0x7e, 0x02, 0xff, 0xff, 0x00, 0x60, 0x86, 0x1f, 0x66, 0xf0, 0xff,
	0xff, 0x00, 0x60, 0x86, 0x1f, 0x76, 0xf0, 0xff, 0xff, 0xfc, 0x67, 0x7e, 0x78, 0x7e, 0xcf, 0xff,
	0xff, 0xfc, 0xe7, 0x7e, 0x38, 0x3e, 0xdf, 0xff, 0xff, 0x0c, 0xe6, 0x67, 0x06, 0x00, 0x3f, 0xff,
	0xff, 0x0c, 0xe6, 0xfb, 0x27, 0x8e, 0x37, 0xff, 0xff, 0x0c, 0xe6, 0xf9, 0x67, 0x9e, 0x33, 0xff,
	0xff, 0x0c, 0xe6, 0x01, 0x1e, 0x86, 0x3d, 0xff, 0xff, 0x0c, 0xe6, 0x01, 0x1e, 0x86, 0x3c, 0xff,
	0xff, 0xfc, 0xe7, 0x86, 0xe7, 0x01, 0xff, 0xff, 0xff, 0xfc, 0x67, 0x86, 0xe7, 0x01, 0xff, 0xff,
	0xff, 0x00, 0x60, 0xe0, 0x81, 0x23, 0x1f, 0xff, 0xff, 0x00, 0x60, 0xe0, 0x81, 0x67, 0x0f, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
};

void OLED_init(){
	u8g2_Setup_sh1106_i2c_128x64_noname_f(&u8g2, U8G2_R0, u8x8_byte, u8x8_delay);
	u8g2_SetI2CAddress(&u8g2, OLED_ADDR);
	u8g2_InitDisplay(&u8g2);
	u8g2_SetPowerSave(&u8g2, 0);
}

void initialize(){
	DDRA &= ~(1 << PA1); // Pin 1 of Port A as input
	DDRA &= ~(1 << PA2); // Pin 2 of Port A as input
	DDRB = 0x00;
	DDRD = 0x00;
	ADC_init();
	I2C_Init();
	USART_Init(9600);
	OLED_init();
}

void valToStr(char *result, int x){
	if(x <= 0){
		result[0] = '0';
		result[1] = '\0';
		return;
	}
	if(x > 100){
		result[0] = '1';
		result[1] = '0';
		result[2] = '0';
		result[3] = '\0';
		return;
	}
	int k,i=0,j;
	while(x>0){
		k = x%10;
		result[i] = k+'0';
		x/=10;
		i++;
	}
	char temp;
	result[i] = '\0';
	for(j=0;j<i/2;j++){
		temp = result[j];
		result[j]= result[i-1-j];
		result[i-1-j] = temp;
	}
}


int main (void)
{
	initialize();

	unsigned char mode = 0, show_QR = 0;
	unsigned char fromB, fromD;
	
	fromD = PIND;
	mode = fromD & 0x80;
	
	while(1){
		while (!mode) {
			fromD = PIND;
			mode = fromD & 0x80;
			
			unsigned char LO_neg = PINA & (1 << PA1);
			unsigned char LO_pos = PINA & (1 << PA2);
			
			if(LO_neg || LO_pos){
				u8g2_ClearBuffer(&u8g2);
				u8g2_DrawXBMP(&u8g2, 0, 0, 128, 64, ECG_PROMPT);
				u8g2_SendBuffer(&u8g2);
			} else {
				u8g2_ClearBuffer(&u8g2);
				for (int x = 0; x < 128; x+=4) {
					unsigned char y = ADC_read(0);
					if (x > 0) {
						u8g2_DrawLine(&u8g2, x - 4, 63 - last_y, x, 63 - y);
					}
					last_y = y;
				}
				u8g2_SendBuffer(&u8g2);
			}
		}
		
		while(mode){
			fromB = PINB;
			fromD = PIND;
			show_QR = fromB & 0x80;
			mode = fromD & 0x80;
			
			if(show_QR){
				u8g2_ClearBuffer(&u8g2);
				u8g2_DrawXBMP(&u8g2, 32, 0, 64, 64, QR_CODE);
				u8g2_SendBuffer(&u8g2);
			} else {
				u8g2_ClearBuffer(&u8g2);
				u8g2_SetFont(&u8g2, u8g2_font_helvB12_tf);
				u8g2_SetFontPosTop(&u8g2);
				int spo2 = fromB & 0x7F;
				int hr = fromD & 0x7C;
				if(!spo2 || !hr){
					u8g2_ClearBuffer(&u8g2);
					u8g2_DrawXBMP(&u8g2, 0, 0, 128, 64, OXYMETER_PROMPT);
					u8g2_SendBuffer(&u8g2);
				} else {
					char fohr[100]="HR: ";
					char ohr[10];
					valToStr(ohr,hr);
					strcat(ohr," bpm");
					strcat(fohr,ohr);
					u8g2_DrawStr(&u8g2, 16, 12, fohr);
					char output[10];
					char final[100] = "SPO2: ";
					valToStr(output,spo2);
					strcat(output," %");
					strcat(final,output);
					u8g2_DrawStr(&u8g2, 16, 36, final);
					u8g2_SendBuffer(&u8g2);
					
					USART_SendString(fohr);
					USART_SendString(final);
				}
			}
		}	
	}
	return 0;
}




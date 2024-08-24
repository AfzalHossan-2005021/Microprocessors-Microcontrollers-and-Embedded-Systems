/*
 * Exprement_01.c
 *
 * Created: 1/21/2024 1:16:54 PM
 * Author : Afzal Hossan
 */ 

#include <avr/io.h>
#define F_CPU 1000000
#include <util/delay.h>

void initialize() {
	// Set PORTA as output for LEDs
	DDRA |= 0x0F;

	// Set PORTB pins for buttons as input
	DDRB &= ~((1 << PB0) | (1 << PB1));
}

// Function to check if the up button is pressed
int isUpButtonPressed() {
	return (PINB & (1 << PB0));
}

// Function to check if the down button is pressed
int isDownButtonPressed() {
	return (PINB & (1 << PB1));
}

// Function to update the LEDs based on the counter value
void updateLEDs(int counter) {
	PORTA = counter & 0x0F;
}

int main() {
	initialize();

	unsigned int counter = 0;
	PORTA = 0x00;

	while (1) {
		// Check if the up button is pressed
		if (isUpButtonPressed()) {
			_delay_ms(100);             // Debounce delay
			if (isUpButtonPressed()) {
				counter+=4;
				updateLEDs(counter);
				while (isUpButtonPressed());  // Wait until the button is released
			}
		}

		// Check if the down button is pressed
		if (isDownButtonPressed()) {
			_delay_ms(100);  // Debounce delay
			if (isDownButtonPressed()) {
				counter-= 4;
				updateLEDs(counter);
				while (isDownButtonPressed());  // Wait until the button is released
			}
		}
	}
	return 0;
}
# Experiment 03: AVR ADC Voltage Display on LCD

## Overview
This experiment demonstrates an AVR-based voltage measurement system using the on-chip ADC and a 16x2 LCD display. The application reads an analog input, converts it to a voltage value, and presents the result on the LCD in real time.

## Functionality
- Configures the AVR ADC to sample analog voltage from the selected input channel.
- Converts the ADC result into a voltage using a 5.00V reference.
- Displays the measured voltage on a 16x2 LCD in the format `Voltage: X.X`.
- Uses a 4-bit LCD interface for data transmission.

## Hardware Connections
- LCD control/data pins are mapped through `lcd.h` using:
  - `D4` -> `PD4`
  - `D5` -> `PD5`
  - `D6` -> `PD6`
  - `D7` -> `PD7`
  - `RS` -> `PC6`
  - `EN` -> `PC7`
- ADC input channel selected by `ADMUX = 0b01000001`.
- `DDRD` and `DDRC` are configured as outputs for the LCD interface.

## Project Structure
- `LCD_demo.atsln` — Atmel Studio solution file.
- `LCD_demo.cproj` — Atmel Studio project file.
- `main.c` — Main application source code.
- `lcd.h` — LCD driver header used by the project.
- `MC Experiment 3.pdf` — Lab documentation and experiment instructions.

## Source Code Summary
- `main()` initializes port directions and ADC registers.
- The ADC is enabled and configured for continuous conversion.
- `dtostrf()` converts the floating-point voltage result into a display string.
- LCD output is updated on the second row while the first row shows the fixed label `Voltage:`.

## Build and Run Instructions
1. Open `Exp-03/LCD_demo/LCD_demo.atsln` or `Exp-03/LCD_demo/LCD_demo/LCD_demo.cproj` in Atmel Studio.
2. Build the project.
3. Program the target AVR device.
4. Power the circuit and apply the analog input signal to the configured ADC input.
5. Observe the voltage reading on the LCD display.

## Notes
- The code assumes a 16 MHz CPU clock (`F_CPU = 16000000UL`).
- The reference voltage is set to 5.00V as defined by `AREF`.
- If the ADC input or reference changes, update `ADMUX` and `AREF` accordingly.
- Use proper current-limiting resistors for the LCD backlight and ensure stable power for the ADC measurement.

---

Prepared for CSE 316: Microprocessors, Microcontrollers, and Embedded Systems.
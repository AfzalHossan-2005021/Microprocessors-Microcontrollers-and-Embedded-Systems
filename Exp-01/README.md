# Experiment 01: AVR Button-Controlled LED Counter

## Overview

This experiment demonstrates a simple AVR microcontroller application where two push buttons increment and decrement a 4-bit LED counter. The design uses an ATmega-series MCU and is implemented in C for Atmel Studio / AVR-GCC.

## Functionality

- `PORTA` pins PA0..PA3 drive four LEDs.
- `PORTB` pins PB0 and PB1 are configured as button inputs.
- Pressing the UP button increments the counter by 4 and updates the LEDs.
- Pressing the DOWN button decrements the counter by 4 and updates the LEDs.
- Basic software debouncing is implemented using a delay and button release wait loop.

## Hardware Connections

- LEDs: `PA0`, `PA1`, `PA2`, `PA3`
- UP button: `PB0`
- DOWN button: `PB1`

> Note: Use appropriate current-limiting resistors for LEDs and pull-down/pull-up resistors for button inputs if required by your target board.

## Project Structure

- `Experiment_01.pdsprj` — Project file for the experiment.
- `Exprement_01/Exprement_01/main.c` — Main source code for the experiment.
- `Exprement_01/Exprement_01/Exprement_01.cproj` — Atmel Studio project configuration.
- `Exprement_01/Exprement_01/Debug/` — Generated debug output files.
- `MC Experiment 1.pdf` — Lab manual or experiment documentation.

## Source Code Summary

The core implementation is in `main.c` and contains:

- `initialize()` to set output and input directions.
- `isUpButtonPressed()` and `isDownButtonPressed()` helper functions.
- `updateLEDs(int counter)` to write the lower 4 bits to `PORTA`.
- A main loop that handles button presses, debounces, and updates the counter.

## Building and Running

1. Open `Experiment_01.pdsprj` or `Exprement_01/Exprement_01/Exprement_01.cproj` in Atmel Studio.
2. Build the project.
3. Program the target AVR device using a supported programmer/debugger.
4. Apply power, then press the buttons to observe the LED counter behavior.

## Notes

- The counter wraps naturally through integer overflow/underflow when incremented or decremented by 4.
- Adjust timing or add hardware debouncing if noisy button behavior is observed.

---

Prepared for CSE 316: Microprocessors, Microcontrollers, and Embedded Systems.

# Experiment 02: AVR 8x8 LED Matrix Display

## Overview

This experiment implements an 8x8 LED matrix display using an AVR microcontroller. The project animates characters on the matrix by scanning rows and writing column patterns in software.

## Key Features

- Controls an 8x8 LED matrix using `PORTB` for row selection and `PORTA` for column data.
- Displays pre-defined patterns for letters A–Z and digits 0–9.
- Includes animated rotation effects and static display modes.
- Demonstrates multiplexing for matrix scanning.

## Hardware Mapping

- `PORTB` (`PB0..PB7`) configured as row outputs.
- `PORTA` (`PA0..PA7`) configured as column outputs.
- Active row drive uses the `rows` array: `{1,2,4,8,16,32,64,128}`.
- Column patterns are inverted with `~` to match common-cathode common-anode wiring depending on matrix type.

## Code Structure

- `Experiment_02/Experiment_02/main.c` — main experiment source file.
- `Generate_hex_array.cpp` — helper program to generate hex arrays for display patterns.
- `Generate_hex_string.cpp` — helper program to convert text strings to hex patterns.
- `MC Exp 2 - LED Matrix.pdf` — lab manual or documentation.
- `MC Experiment 2.pdf` — experiment notes.
- `RoboIndias8x8 Dot LED Matrix Hex Values for Arduino.pdf` — reference for hex mapping.

## Source Functionality

- `move_show()` — continuously animates a moving pattern from `rotateALPHA` using row/column scanning.
- `static_show()` — displays a single static character pattern from `ALPHA`.
- `flush_show()` — alternates display on and off to demonstrate refresh behavior.
- The `main()` function initializes both row and column ports as outputs and calls `move_show()`.

## Build and Run Instructions

1. Open `Experiment_02.pdsprj` or `Experiment_02/Experiment_02/Experiment_02.cproj` in Atmel Studio.
2. Build the project.
3. Program the AVR target device with the generated HEX file.
4. Power the circuit and observe the LED matrix animation.

## Notes

- The code is configured for a 1 MHz CPU clock via `F_CPU`.
- The animation is driven by repeated 2 ms delays in the matrix scan loop.
- Uncomment `static_show()` or `flush_show()` in `main()` to test alternate display modes.
- Adjust wiring and signal inversion based on the specific LED matrix hardware.

---

Prepared for CSE 316: Microprocessors, Microcontrollers, and Embedded Systems.

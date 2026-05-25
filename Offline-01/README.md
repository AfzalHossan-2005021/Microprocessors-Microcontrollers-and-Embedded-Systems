# Offline-01: 8086 Assembly Practice

## Overview
This folder contains two 8086 assembly exercises for offline practice in CSE 316. Each problem uses DOS interrupt services and demonstrates basic character input, validation, and branching logic in assembly language.

## Contents
- `Problem1.asm` — Accepts a single ASCII character and classifies it as a number, uppercase letter, lowercase letter, or a non-alphanumeric character.
- `Problem2.asm` — Accepts three lowercase letters and compares them to find the greatest character value, with validation for lowercase input.
- `July_2023_CSE_316_Assembly_Offline_1.pdf` — Offline exercise instructions or supporting documentation.

## Problem 1: Character Classification
- Prompts the user: `Input a single printable ASCII character:`
- Reads one character using DOS interrupt `INT 21h`, function `01h`.
- Checks character ranges and prints one of these messages:
  - `Number`
  - `Uppercase letter`
  - `Lowercase letter`
  - `Not an alphanumeric value`
- Demonstrates conditional jumps, range checking, and DOS string output via `AH = 9`.

## Problem 2: Lowercase Letter Comparison
- Prompts the user: `Input three lowercase letters:`
- Reads three characters with `INT 21h`, function `01h`.
- Prints spaces between the inputs and a newline after the third character.
- Validates that each input is a lowercase letter (`'a'` to `'z'`).
- If all inputs are valid, compares the characters and outputs the greatest character.
- If any input is invalid, prints `Invalid input`.
- If all three letters are equal, prints `All letters are equal`.

## Build and Run
1. Assemble `Problem1.asm` or `Problem2.asm` with an 8086-compatible assembler such as MASM or TASM.
2. Link the generated object file into a DOS executable.
3. Run the executable in DOS, DOSBox, or a compatible emulator.

## Notes
- Both programs use `.MODEL SMALL`, `.STACK 100H`, and DOS interrupt `INT 21H` services.
- The programs assume a text-mode DOS environment for character I/O.
- These exercises illustrate core assembly concepts: registers, comparisons, branching, and basic I/O.

---

Prepared for CSE 316: Microprocessors, Microcontrollers, and Embedded Systems.
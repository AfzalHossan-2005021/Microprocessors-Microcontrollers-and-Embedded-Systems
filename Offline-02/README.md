# Offline-02: 8086 Assembly Practice

## Overview

This folder contains offline assembly practice programs for CSE 316. The exercises use 8086 assembly language with DOS interrupt services to perform console input/output, numeric processing, and conditional branching.

## Contents

- `Problem1.asm` — Classifies a single ASCII character as a number, uppercase letter, lowercase letter, or non-alphanumeric.
- `Problem2.asm` — Calculates the digit sum of a decimal number entered by the user.
- `Problem2_R.asm` — Revised version of the digit-sum program with a cleaner recursive sum implementation.
- `July_2023_CSE_316_Assembly_Offline_2.pdf` — Offline assignment handout or supporting documentation.

## Problem 1: Character Classification

- Prompt: `Input two numbers(space separated) : $`.
- Reads a single printable ASCII character using DOS interrupt `INT 21h` function `01h`.
- Checks ranges and prints one of the following labels:
  - `Number`
  - `Uppercase letter`
  - `Lowercase letter`
  - `Not an alphanumeric value`
- Demonstrates:
  - `.MODEL SMALL`
  - Data segment initialization
  - Character comparisons
  - Branching with `JB`, `JA`, `JE`
  - DOS string output via `AH = 9`

## Problem 2: Digit Sum Calculator

- Prompt: `Input a number : $`.
- Reads an integer from character input, parsing digits until a non-digit terminator is entered.
- Computes the sum of digits in the entered number.
- Prints the result using DOS character output.
- Demonstrates:
  - Numeric input parsing with ASCII subtraction
  - Integer arithmetic in registers
  - Recursive procedure design in assembly
  - Stack usage for function calls and return values

## Difference Between `Problem2.asm` and `Problem2_R.asm`

- `Problem2.asm` stores the digit sum in a memory variable and uses a recursive helper that accumulates partial sums through the stack frame.
- `Problem2_R.asm` simplifies the recursion by returning the digit sum directly in `CX`, then storing the result before printing.

## Build and Run

1. Assemble the chosen `.asm` file with MASM, TASM, or another 8086-compatible assembler.
2. Link the object file into a DOS executable.
3. Run the executable in DOS, DOSBox, or a compatible emulator.

## Notes

- These programs target a DOS environment with INT 21h services for console I/O.
- The code uses `.STACK 0FFFFH` and a small memory model.
- Use keyboard input and observe outputs directly in the DOS console.

---

Prepared for CSE 316: Microprocessors, Microcontrollers, and Embedded Systems.

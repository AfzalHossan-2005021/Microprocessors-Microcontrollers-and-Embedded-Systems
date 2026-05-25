<div align="center">

# 🖥️ Microprocessors, Microcontrollers, and Embedded Systems
**Course Code: CSE 316**

[![C](https://img.shields.io/badge/Language-C-00599C?style=for-the-badge&logo=c&logoColor=white)]()
[![Assembly](https://img.shields.io/badge/Language-Assembly-8A2BE2?style=for-the-badge&logo=nasm&logoColor=white)]()
[![Microcontroller](https://img.shields.io/badge/Platform-ATmega32-blue?style=for-the-badge&logo=microchip)]()
[![Simulation](https://img.shields.io/badge/Tool-Proteus-red?style=for-the-badge)]()
[![IDE](https://img.shields.io/badge/IDE-Microchip_Studio-007ACC?style=for-the-badge)]()

*A comprehensive collection of lab experiments, assembly coding assignments, and a full-fledged IoT health telemetry project.*

</div>

## 📖 Overview

This repository contains all the coursework, experiments, offline assembly assignments, and the final embedded systems project designed for the **CSE 316: Microprocessors, Microcontrollers, and Embedded Systems** course. The focus of this course spans from low-level Assembly programming to high-level C-based embedded application development, hardware interfacing, and system simulation using the **ATmega32** architecture.

---

## 🗂️ Repository Structure

### 🧪 Lab Experiments (C Programming & Hardware Interfacing)
These folders contain Atmel/Microchip Studio projects and Proteus simulations demonstrating core microcontroller functionalities.
- **[Exp-01](./Exp-01):** Introduction to digital I/O. Basic LED blinking, button interfacing, and port manipulation logic.
- **[Exp-02](./Exp-02):** Advanced digital logic, generating hex patterns/strings, and 7-segment display control.
- **[Exp-03](./Exp-03):** Analog-to-Digital Conversion (ADC) and graphical/alphanumeric LCD interfacing (`LCD_demo`).

### 📝 Offline Assignments (Assembly Language)
Assignments focusing on microprocessor architecture, memory management, and instruction set architectures.
- **[Offline-01](./Offline-01):** Foundational Assembly programming, arithmetic operations, and basic branching/logic.
- **[Offline-02](./Offline-02):** Advanced Assembly routines, looping structures, arrays, and memory operations.

### 🚀 Final Project: IoT Health Monitoring System
- **[Project](./Project):** An integrated IoT Medical Health Monitoring System featuring real-time biometrics (SpO2, Heart Rate via MAX30100/02, ECG via AD8232), an OLED UI (U8g2), master-slave MCU bridging (SPI/I2C), and GSM-based cloud telemetry (SIM800L). 
  > *For an in-depth breakdown of the architecture, see the [Project README](./Project/README.md).*

---

## 🛠️ Tools & Technologies

*   **Microcontrollers:** AVR ATmega32, Arduino Uno/Nano (as peripheral bridge in the project).
*   **Languages:** `C/C++` (for embedded firmware), `Assembly` (for offline assignments).
*   **IDEs/Compilers:** Microchip / Atmel Studio 7.0, Arduino IDE.
*   **Simulation & Prototyping:** Proteus Professional 8.x.
*   **Communication Protocols:** SPI, I2C, UART/USART, AT Commands.

---

## ⚙️ How to Run & Simulate

### C / MCU Projects (Experiments & Project)
1. Open the respective `.atsln` or `.cproj` files in **Microchip Studio (Atmel Studio)**.
2. Build the project solution mapped to the ATmega32 target.
3. The newly compiled `.hex` files will be placed inside the `Debug/` folder.

### Assembly Projects
1. You can run the `.asm` code on appropriate 8086/AVR emulators depending on the target architecture of the offline assignments (e.g., using EMU8086 or AVR Studio).

### Hardware Simulation (Proteus)
1. Ensure **Proteus Professional** is installed.
2. Open any `.pdsprj` file found in the experiment or project directories.
3. Double-click the virtual ATmega32 MCU in the Proteus schematic and assign the `Program File` path to the newly compiled `.hex` file.
4. Play the simulation to see the virtual hardware in action.

---

<div align="center">
  <i>Developed for CSE 316 Level-3 Term-I academic coursework.</i>
</div>

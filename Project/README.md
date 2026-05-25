<div align="center">

# 🏥 IoT Health Monitoring & Telemetry System
**Microprocessors, Microcontrollers, and Embedded Systems (CSE 316)**

[![Microcontroller](https://img.shields.io/badge/Microcontroller-ATmega32-blue?style=for-the-badge&logo=microchip)](https://www.microchip.com/)
[![Arduino](https://img.shields.io/badge/Helper-Arduino_Nano/Uno-00979C?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![Language](https://img.shields.io/badge/Language-C/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)](https://isocpp.org/)
[![Simulation](https://img.shields.io/badge/Simulation-Proteus-red?style=for-the-badge)](#)

*An advanced embedded systems project featuring real-time biometrics, SPI/I2C communication, and GSM-based cloud telemetry.*

</div>

## 📑 Table of Contents
- [About the Project](#-about-the-project)
- [System Architecture](#-system-architecture)
- [Hardware Components](#-hardware-components)
- [Software & Libraries](#-software--libraries)
- [Project Structure](#-project-structure)
- [Protocols Used](#-protocols-used)
- [Getting Started](#-getting-started)
- [Acknowledgements](#-acknowledgements)

---

## 📖 About the Project

This project simulates and implements an integrated **IoT Medical Health Monitoring System**. Built on the **ATmega32** AVR microcontroller and supported by the **Arduino** ecosystem as a peripheral bridge, the system captures vital signs, processes the signals securely, displays them via an OLED interface, and transmits telemetry data via a GSM module to a remote server. 

### Key Features
- **Real-Time Vitals Tracking:** Simultaneous SpO2, Heart Rate, and ECG acquisition.
- **Microcontroller Bridging:** Master-slave communication seamlessly bridging Arduino peripherals to the ATmega32 MCU.
- **Rich OLED Interface:** Embedded graphical display using the robust U8g2 graphics architecture.
- **IoT Telemetry:** GSM/GPRS capabilities to push health data securely to a remote SIM server.
- **Robust DSP:** Embedded digital signal processing for beat detection and noise filtering.

---

## 🏗️ System Architecture

1. **Sensing Layer:** 
   - `MAX30100/MAX30102` captures photoplethysmogram (PPG) signals.
   - `AD8232` captures electrocardiogram (ECG) data.
2. **Intermediate/Processing Layer:**
   - **Arduino** handles complex sensor abstractions and filtering.
   - **ATmega32 (Master MCU)** handles core state machine, user interface, and overall system synchronization via **SPI** and **I2C**.
3. **Output & Telemetry Layer:**
   - **Local:** Graphical OLED (I2C/SPI) outputting wave shapes and BPM values.
   - **Remote:** `SIM800L` sending formatted payloads to HTTP/TCP servers.

---

## 🔌 Hardware Components

| Module | Description | Interface |
|:---:|:---:|:---:|
| **ATmega32** | Core AVR Microcontroller | MCU |
| **Arduino Uno/Nano** | Peripheral Bridge & DSP Hand-off | SPI / I2C |
| **MAX30100 / 02** | Pulse Oximeter & Heart Rate Sensor | I2C |
| **AD8232** | ECG Analog Front-End Biopotential Sensor | Analog / ADC |
| **OLED Display** | SSD1306 Graphic Display | I2C / SPI |
| **SIM800L** | GSM/GPRS Module | UART (AT Commands) |

---

## 📁 Project Structure

```text
Project/
├── 📂 Working/                     # 🌟 THE FINAL INTEGRATED SYSTEM 
│   ├── SIM_Server/                 # GSM IoT server push mechanics
│   ├── SIM800L_with_Arduino/       # AT Command interface for SIM800L
│   ├── SPO2_Arduino_to_ATMEGA32/   # Arduino MCU bridging logic
│   ├── u8g2_demo/                  # Final OLED UI integration
│   └── TestForProject.pdsprj       # Integrated Proteus Simulation File
├── 📂 MAX30100/                    # Core MAX30100/02 C/C++ Drivers
│   ├── Filters & BeatDetector      # Embedded DSP logic
│   └── SpO2Calculator              # Arithmetic logic for Oxygen saturation
├── 📂 AD8232_demo/                 # ECG interface implementation
├── 📂 SPI_working/ & SPI_Slave/    # Inter-MCU SPI communication protocols
└── 📂 u8g2_demo/                   # Standalone OLED rendering demo
```

---

## 📡 Protocols Used
- **I2C:** Sensor configuration, FIFO management, and short-distance OLED display updating.
- **SPI:** High-speed data bridging between Arduino and AVR ATmega32 nodes.
- **UART / USART:** Serial debugging and AT-command execution for the SIM800L telecommunication node.
- **ADC:** Analog signal sampling mapping the AD8232 signals into discrete variables.

---

## 🚀 Getting Started

### 1. Hardware Simulation (Proteus)
- Navigate to the `Working/` directory.
- Open `TestForProject.pdsprj` using **Proteus Professional**.
- Ensure the compiled `.hex` files from Atmel Studio and Arduino IDE are linked to the respective virtual microcontrollers.
- Run the simulation to see real-time UI mapping and logic flow.

### 2. Physical Deployment
- **Arduino:** Open `.ino` sketches via the **Arduino IDE**. Upload the sensor bridge codes (`SPO2_Arduino_to_ATMEGA32`).
- **AVR / ATmega32:** Open `.cproj` / `.atsln` projects in **Microchip Studio (Atmel Studio)**.
  - Compile the C code in the `u8g2_demo/` or `Working/` directory.
  - Flash the `.hex` to the AVR using an ISP programmer (e.g., USBasp).

### 3. Dependencies
- Install the `U8g2` library directly from the Arduino Library Manager or link it in Microchip Studio.
- Ensure proper configuration of your serial interface (`9600` baud by default) for `SIM800L` testing.

---

## 🤝 Acknowledgements
This project was conceptualized and developed as part of the **CSE 316: Microprocessors, Microcontrollers, and Embedded Systems** course. Special thanks to the open-source community for maintaining robust sensor APIs.

<div align="center">
  <sub>Built with ❤️ for Embedded Systems Engineering.</sub>
</div>

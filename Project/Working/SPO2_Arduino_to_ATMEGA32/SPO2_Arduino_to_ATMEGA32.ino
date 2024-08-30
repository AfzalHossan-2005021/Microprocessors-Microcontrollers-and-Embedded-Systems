/*
Arduino-MAX30100 oximetry / heart rate integrated sensor library
Copyright (C) 2016  OXullo Intersecans <x@brainrapers.org>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

// The LEDs currents must be set to a level that avoids clipping and maximises the dynamic range
#define LED_CURRENT MAX30100_LED_CURR_27_1MA

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;
uint32_t tsLastReport = 0;

const int bit0 = 2;  // Define the digital pins you want to use
const int bit1 = 3;
const int bit2 = 4;
const int bit3 = 5;
const int bit4 = 6;
const int bit5 = 7;
const int bit6 = 8;

const int bbit0 = 9;
const int bbit1 = 10;
const int bbit2 = 11;
const int bbit3 = 12;
const int bbit4 = 13;


void writeBinaryToPins(int value) {
  digitalWrite(bit0, (value & 0x01) ? HIGH : LOW);
  digitalWrite(bit1, (value & 0x02) ? HIGH : LOW);
  digitalWrite(bit2, (value & 0x04) ? HIGH : LOW);
  digitalWrite(bit3, (value & 0x08) ? HIGH : LOW);
  digitalWrite(bit4, (value & 0x10) ? HIGH : LOW);
  digitalWrite(bit5, (value & 0x20) ? HIGH : LOW);
  digitalWrite(bit6, (value & 0x40) ? HIGH : LOW);
}

void writeBpBinaryToPins(int value) {
  digitalWrite(bbit0, (value & 0x04) ? HIGH : LOW);
  digitalWrite(bbit1, (value & 0x08) ? HIGH : LOW);
  digitalWrite(bbit2, (value & 0x10) ? HIGH : LOW);
  digitalWrite(bbit3, (value & 0x20) ? HIGH : LOW);
  digitalWrite(bbit4, (value & 0x40) ? HIGH : LOW);
}


void setup()
{
    Serial.begin(9600);

    Serial.print("Initializing pulse oximeter..");

    // Initialize the PulseOximeter instance
    // Failures are generally due to an improper I2C wiring, missing power supply or wrong target chip
    if (!pox.begin()) {
        Serial.println("FAILED");
        while(1);
    } else {
        Serial.println("SUCCESS");
    }

    // The default current for the IR LED is 50mA and it could be changed
    //   by uncommenting the following line. Check MAX30100_Registers.h for all the available options.
    pox.setIRLedCurrent(LED_CURRENT);

    pinMode(bit0, OUTPUT);  // Set pins as output
    pinMode(bit1, OUTPUT);
    pinMode(bit2, OUTPUT);
    pinMode(bit3, OUTPUT);
    pinMode(bit4, OUTPUT);
    pinMode(bit5, OUTPUT);
    pinMode(bit6, OUTPUT);

    pinMode(bbit0, OUTPUT);  // Set pins as output
    pinMode(bbit1, OUTPUT);
    pinMode(bbit2, OUTPUT);
    pinMode(bbit3, OUTPUT);
    pinMode(bbit4, OUTPUT);
}

void loop()
{
    // Make sure to call update as fast as possible
    pox.update();

    // Asynchronously dump heart rate and oxidation levels to the serial
    // For both, a value of 0 means "invalid"
    if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
        uint8_t spo2 = pox.getSpO2();
        float hr = pox.getHeartRate();
        int nhr = hr;
        Serial.print("Heart rate: ");
        Serial.print(nhr);
        Serial.print("bpm\n");
        Serial.print("SpO2: ");
        Serial.print(spo2);
        Serial.print("%\n");
        if(spo2 > 0 && spo2 <= 100){
          writeBinaryToPins(spo2);
        } else {
          writeBinaryToPins(0);
        }
        if(nhr > 0 && nhr <= 100){
          writeBpBinaryToPins(nhr);
        } else {
          writeBpBinaryToPins(0);
        }

        tsLastReport = millis();
    }
}

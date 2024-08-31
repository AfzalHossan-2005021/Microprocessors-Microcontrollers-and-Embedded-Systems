#include <SoftwareSerial.h>

// Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2); // SIM800L Tx & Rx is connected to Arduino #3 & #2

// Constants
const int MIN_VALUE = 40;
const int MAX_VALUE = 100;
const int NUM_READINGS = 50;

void setup() {
  //Set pins 4 to 10 as inputs
  for (int pin = 4; pin <= 9; pin++) {
    pinMode(pin, INPUT);
  }
  pinMode(11,INPUT);

  // Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  // Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);
  
  delay(1000);

  // Initialize variables
  int readings[NUM_READINGS];
  int sum = 0;
  int count = 0;

  // Read values until 100 valid values are collected
  while (count < NUM_READINGS) {
    // Read the 7-bit value from pins 4 to 10
    int value = 0;
    for (int pin = 4; pin <= 9; pin++) {
      // Read the state of the pin and shift the bits
      value |= digitalRead(pin) << (pin - 4);
    }
    value |= digitalRead(11) << 6;

    // Check if the value is within the desired range
    if (value >= MIN_VALUE && value <= MAX_VALUE) {
      // Store the valid value
      readings[count] = value;
      sum += value;
      count++;
    }
    delay(100); // Add a small delay to prevent too fast readings
  }

  // // Calculate the average
  int average = sum / NUM_READINGS;

  // Send the average value via SMS
  mySerial.println("AT+CMGF=1"); // Set SMS mode to text
  updateSerial();
  mySerial.println("AT+CMGS=\"+8801601638912\""); // Change ZZ with country code and xxxxxxxxxxx with phone number to SMS
  updateSerial();
  mySerial.print("Your SPO2: "); 
  mySerial.print(average); // Send the average value
  mySerial.print("%");
  updateSerial();
  mySerial.write(26); // Send Ctrl-Z to indicate the end of the message
}

void loop() {
  // Nothing to do here
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read()); // Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read()); // Forward what Software Serial received to Serial Port
  }
}

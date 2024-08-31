#include <SoftwareSerial.h>

// Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(3, 2);  // SIM800L Tx & Rx is connected to Arduino #3 & #2

String senderNumber;     // Variable to store the sender's phone number
String receivedMessage;  // Variable to store the received message content

const int MIN_VALUE = 90;
const int MAX_VALUE = 100;
const int NUM_READINGS = 50;
int request_received = 0;
int readings[NUM_READINGS];
int sample_index = 0;
int sample_count = 0;

void sample_spo2() {
  int value = 0;
  for (int pin = 4; pin <= 9; pin++) {
    // Read the state of the pin and shift the bits
    value |= digitalRead(pin) << (pin - 4);
  }
  value |= digitalRead(11) << 6;

  // Check if the value is within the desired range
  if (value >= MIN_VALUE && value <= MAX_VALUE) {
    // Store the valid value
    readings[sample_index] = value;
    sample_index++;
    if (sample_count < NUM_READINGS) {
      sample_count++;
    }
    if (sample_index >= NUM_READINGS) {
      sample_index = 0;
    }
  }
}

int avg_spo2() {
  int sum = 0;
  for (int i = 0; i < sample_count; i++) {
    sum += readings[i];
  }
  return sum / sample_count;
}

void setup() {
  //Set pins 4 to 10 as inputs
  for (int pin = 4; pin <= 9; pin++) {
    pinMode(pin, INPUT);
  }
  pinMode(11, INPUT);

  // Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);

  // Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT");  // Once the handshake test is successful, it will back to OK
  updateSerial();

  mySerial.println("AT+CMGF=1");  // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0");  // Decides how newly arrived SMS messages should be handled
  updateSerial();
}

void loop() {
  // Check if there's a new SMS available
  if (mySerial.available()) {
    Serial.println("Received request for SpO2 measurement.");
    // Read the sender's phone number
    mySerial.readStringUntil('"');  // Skip the opening quotation mark
    senderNumber = mySerial.readStringUntil('"');

    // Skip the next two strings (date and time)
    mySerial.readStringUntil('\n');

    // Read the content of the SMS
    receivedMessage = mySerial.readStringUntil('\0');

    // Print sender's phone number and received message to the serial monitor
    Serial.print("Sender's Number: ");
    Serial.println(senderNumber);
    Serial.print("Received Message: ");
    Serial.println(receivedMessage);

    // Check if the received message is "GET SPO2"
    if (receivedMessage.indexOf("GET SPO2") != -1) {
      // Perform actions to measure SpO2 and respond accordingly
      Serial.println("Performing measurement...");
      // Placeholder function to get SpO2 value
      int average = avg_spo2();
      sendSMS(senderNumber, "SpO2 Measurement: " + String(average) + "%");  // Send SpO2 value to the sender
    }
  }
  sample_spo2();
  delay(500);
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());  // Forward data from Arduino IDE serial monitor to SIM800L
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());  // Forward data from SIM800L to Arduino IDE serial monitor
  }
}

void sendSMS(String recipient, String message) {
  Serial.println("Sending SPO2 to requester...");
  mySerial.print("AT+CMGS=\"");
  mySerial.print(recipient);
  mySerial.println("\"");
  updateSerial();
  mySerial.println(message);
  updateSerial();
  mySerial.write(26);  // End of message character (ASCII code for CTRL+Z)
}

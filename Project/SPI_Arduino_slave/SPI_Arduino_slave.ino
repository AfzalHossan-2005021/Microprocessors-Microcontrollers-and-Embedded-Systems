#include <SPI.h>

#define LED 7  

volatile boolean received_from_master;
volatile byte slave_received, slave_sent;

void setup() {
  Serial.begin(9600);
  pinMode(LED,OUTPUT); 

  pinMode(MISO, OUTPUT);  //Sets MISO as OUTPUT (Have to Send data to Master IN)

  SPCR |= _BV(SPE);  //Turn on SPI in Slave Mode

  received_from_master = false;

  SPI.attachInterrupt();  //Interuupt ON is set for SPI commnucation
}


ISR(SPI_STC_vect)  //Inerrrput routine function
{
  slave_received = SPDR;  // Value received from master if store in variable slave_received

  received_from_master = true;  //Sets received as True
}


void loop() {
  if (received_from_master)  //Logic to SET LED ON OR OFF depending upon the value recerived from master
  {
    Serial.println(slave_received);

    SPDR = slave_sent;  //Sends the x value to master via SPDR

    received_from_master = false;
  }
}
#include <SPI.h>  //Library for SPI

unsigned char x = 0;

void setup(void) {

  Serial.begin(115200);  //Starts Serial Communication at Baud Rate 115200

  SPI.begin();  //Begins the SPI commnuication

  SPI.setClockDivider(SPI_CLOCK_DIV64);  //Sets clock for SPI communication at 2 (16/2=8Mhz)

  digitalWrite(SS, HIGH);  // Setting SlaveSelect as HIGH (So master doesnt connnect with slave)
}


void loop(void) {

  byte Mastersend, Mastereceive;

  Mastersend = x++;

  digitalWrite(SS, LOW);  //Starts communication with Slave connected to master

  Mastereceive = SPI.transfer(Mastersend);  //Send the mastersend value to slave also receives value from slave

  Serial.println(Mastereceive);

  delay(1000);
}

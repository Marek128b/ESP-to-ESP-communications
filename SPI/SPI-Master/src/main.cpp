#include <Arduino.h>
#include <SPI.h>

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;

void setup()
{
  // set the slaveSelectPin as an output:
  pinMode(slaveSelectPin, OUTPUT);
  // initialize SPI:
  SPI.begin();
}

void loop()
{
  String data_tx_str = "hkjgahkgnkja";
  uint8_t* data_tx = data_tx_str.toInt();
  SPI.writeBytes(data_tx, sizeof(data_tx));
}

void digitalPotWrite(int value)
{
  // take the SS pin low to select the chip:
  digitalWrite(slaveSelectPin, LOW);
  delay(100);
  //  send in the address and value via SPI:
  SPI.transfer(value);
  delay(100);
  // take the SS pin high to de-select the chip:
  digitalWrite(slaveSelectPin, HIGH);
}
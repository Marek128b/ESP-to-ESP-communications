#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> doc;

//https://randomnerdtutorials.com/esp32-lora-rfm95-transceiver-arduino-ide/

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

int counter = 0;

void setup()
{
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("LoRa Sender");

  //setup LoRa transceiver module
  LoRa.setPins(ss, rst, dio0);

  //signalBandwidth - signal bandwidth in Hz, defaults to 125E3.
  //Supported values are 7.8E3, 10.4E3, 15.6E3, 20.8E3, 31.25E3, 41.7E3, 62.5E3, 125E3, 250E3, and 500E3
  LoRa.setSignalBandwidth(7.8E3);

  //replace the LoRa.begin(---E-) argument with your location's frequency
  //433E6 for Asia
  //866E6 for Europe
  //915E6 for North America
  while (!LoRa.begin(866E6))
  {
    Serial.println(".");
    delay(500);
  }
  // Change sync word (0xF3) to match the receiver
  // The sync word assures you don't get LoRa messages from other LoRa transceivers
  // ranges from 0x00-0xFF
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

  // Add values in the document
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;

  // Add an array.
  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);
}

void loop()
{
  String outData;
  serializeJson(doc, outData);

  Serial.print("Sending data: ");
  Serial.println(outData);

  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(outData);
  LoRa.endPacket();

  doc["time"] = counter;
  counter++;

  delay(2000);
}
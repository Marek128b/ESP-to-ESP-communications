#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> doc;

//define the pins used by the transceiver module
#define ss 5
#define rst 14
#define dio0 2

void setup()
{
  //initialize Serial Monitor
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("LoRa Receiver");

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
}

void loop()
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize)
  {
    // read packet
    String LoRaData;
    while (LoRa.available())
    {
      LoRaData = LoRa.readString();
    }

    // print RSSI of packet
    Serial.print("Packet received RSSI: ");
    Serial.print(LoRa.packetRssi());
    Serial.print(", Data: ");

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, LoRaData);

    // Test if parsing succeeds.
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    else
    {
      Serial.println(LoRaData);
      // Fetch values.
      // Most of the time, you can rely on the implicit casts.
      // In other case, you can do doc["time"].as<long>();
      const char *sensor = doc["sensor"];
      long time = doc["time"];
      double latitude = doc["data"][0];
      double longitude = doc["data"][1];

      // Print values.
      Serial.println(sensor);
      Serial.println(time);
      Serial.println(latitude, 6);
      Serial.println(longitude, 6);
      Serial.println();
    }
  }
}
// WireSlave Receiver
// by Gutierrez PS <https://github.com/gutierrezps>
// ESP32 I2C slave library: <https://github.com/gutierrezps/ESP32_I2C_Slave>
// based on the example by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the WireSlave library for ESP32.
// Receives data as an I2C/TWI slave device; data must
// be packed using WirePacker.
// Refer to the "master_writer" example for use with this

#include <Arduino.h>
#include <Wire.h>
#include <WireSlave.h>

#include <ArduinoJson.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

StaticJsonDocument<200> doc;
// ######################################################################################################################################
void receiveEvent(int howMany);
// ######################################################################################################################################

void setup()
{
  Serial.begin(115200);

  bool success = WireSlave.begin(SDA_PIN, SCL_PIN, I2C_SLAVE_ADDR);
  if (!success)
  {
    Serial.println("I2C slave init failed");
  }

  WireSlave.onReceive(receiveEvent);
}

void loop()
{
  // the slave response time is directly related to how often
  // this update() method is called, so avoid using long delays
  // inside loop(), and be careful with time-consuming tasks
  WireSlave.update();

  // let I2C and other ESP32 peripherals interrupts work
  delay(1);
}

// function that executes whenever a complete and valid packet
// is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  Serial.print("Receiving: ");
  String receiveString;
  while (1 < WireSlave.available()) // loop through all but the last byte
  {
    char c = WireSlave.read(); // receive byte as a character
    receiveString += c;
  }
  Serial.println(receiveString);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, receiveString);

  // Test if parsing succeeds.
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  const char *sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
}
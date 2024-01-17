// prints out the MAC Address from esp32 and esp8266
#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

void setup()
{
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
}

void loop()
{
}
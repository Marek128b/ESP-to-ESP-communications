#include <Arduino.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> doc;

void setup()
{
  Serial.begin(115200);
  Serial2.begin(5000000); //5Mb/s => 625kB/s

  Serial.println("Receiver");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial2.available() > 0)
  {
    String input = "";
    while (true)
    {
      char in = Serial2.read();
      input += in;
      if (in == '\n')
      {
        break;
      }
    }

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, input);

    // Test if parsing succeeds.
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    else
    {
      Serial.println(input);
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
    }
  }
}
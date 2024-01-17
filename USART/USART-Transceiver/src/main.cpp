#include <Arduino.h>
#include <ArduinoJson.h>

/* The lines `StaticJsonDocument<200> docRX;` and `StaticJsonDocument<200> docTX;` are declaring two
instances of the `StaticJsonDocument` class. */
StaticJsonDocument<200> docRX;
StaticJsonDocument<200> docTX;

unsigned long last = 0;
const int interval = 2000;

void setup()
{ 
  // Initialize Serial port
  Serial.begin(115200);
  Serial2.begin(5000000);

  Serial.println("Transceiver");

  // Add values in the document
  docTX["sensor"] = "gps";
  docTX["time"] = 1351824120;

  // Add an array.
  JsonArray data = docTX.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);
}

void loop()
{
  static int counter = 0;
  if (millis() - last >= interval) //Transmitter
  {
    last = millis();

    Serial.print("Transmitting: ");
    // Generate the minified JSON and send it to the Serial port.
    String outData;
    serializeJson(docTX, outData);
    Serial.print(outData);
    Serial2.println(outData);

    Serial.println();

    const char *sensor = docTX["sensor"];
    long time = docTX["time"];
    double latitude = docTX["data"][0];
    double longitude = docTX["data"][1];

    Serial.println(sensor);
    Serial.println(time);
    Serial.println(latitude);
    Serial.println(longitude);

    docTX["time"] = counter;

    counter++;
  }

  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  // put your main code here, to run repeatedly:
  if (Serial2.available() > 0) //receiver
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
    DeserializationError error = deserializeJson(docRX, input);

    // Test if parsing succeeds.
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    else
    {
      Serial.print("Receiving: ");
      Serial.println(input);
      // Fetch values.
      // Most of the time, you can rely on the implicit casts.
      // In other case, you can do doc["time"].as<long>();
      const char *sensor = docRX["sensor"];
      long time = docRX["time"];
      double latitude = docRX["data"][0];
      double longitude = docRX["data"][1];

      // Print values.
      Serial.println(sensor);
      Serial.println(time);
      Serial.println(latitude, 6);
      Serial.println(longitude, 6);
    }
  }
}
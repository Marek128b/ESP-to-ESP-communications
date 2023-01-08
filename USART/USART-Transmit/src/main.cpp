#include <Arduino.h>
#include <ArduinoJson.h>

StaticJsonDocument<200> doc;

void setup()
{
  // Initialize Serial port
  Serial.begin(115200);
  Serial2.begin(115200);

  Serial.println("Transmitter");

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
  static int counter = 0;
  // Generate the minified JSON and send it to the Serial port.
  String outData;
  serializeJson(doc, outData);
  Serial.println(outData);
  Serial2.println(outData);

  Serial.println();

  const char *sensor = doc["sensor"];
  long time = doc["time"];
  double latitude = doc["data"][0];
  double longitude = doc["data"][1];

  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude);
  Serial.println(longitude);

  doc["time"] = counter;

  counter++;

  serializeJson(doc, Serial);
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}

  delay(2000);
}
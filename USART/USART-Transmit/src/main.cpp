#include <Arduino.h>
#include <ArduinoJson.h>

/* `StaticJsonDocument<200> doc;` is declaring a variable named `doc` of type `StaticJsonDocument` with
a capacity of 200 bytes. This variable will be used to store the JSON data. */
StaticJsonDocument<200> doc;

void setup()
{
  // initialize Serial port
  Serial.begin(115200);
  // initialize Serial Port2 for communicating with the second ESPs
  Serial2.begin(5000000);

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
  // Generate the JSON document, send it to the Serial port and to the other ESP via Serial2.
  String outData;
  serializeJson(doc, outData);
  // sending data to the Srial Monitor
  Serial.println(outData);
  // sending the data to the second ESP
  Serial2.println(outData);

  Serial.println();

  /* The values from the JSON document get stored in the `doc` variable. */
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
  // The above line prints:
  // {"sensor":"gps","time":1351824120,"data":[48.756080,2.302038]}
  delay(2000);
}
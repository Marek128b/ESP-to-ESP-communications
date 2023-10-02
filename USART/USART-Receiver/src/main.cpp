#include <Arduino.h>
#include <ArduinoJson.h>

/* `StaticJsonDocument<200> doc;` is declaring a variable named `doc` of type `StaticJsonDocument` with
a capacity of 200 bytes. This variable will be used to store the JSON data received from the serial
communication. The capacity of the document determines the maximum size of the JSON data that can be
stored in it. */
StaticJsonDocument<200> doc;

void setup()
{
  // initialize Serial Port
  Serial.begin(115200);
  // inizialize receiving Serial Port2
  Serial2.begin(5000000); //5Mb/s => 625kB/s

  Serial.println("Receiver");
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Serial2.available() > 0)
  {
    /* The code snippet is reading characters from the Serial2 port and appending them to the `input`
    string until a newline character (`'\n'`) is encountered. This is typically used to read a
    complete line of data from the serial communication. Once the newline character is found, the
    loop breaks and the `input` string contains the complete line of data received. */
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

    /* The code snippet is checking if there was an error during the deserialization of the JSON
    document. If there was an error, it prints an error message to the Serial monitor indicating
    that the deserialization failed, along with the specific error message. It then returns from the
    function, effectively stopping further execution of the code. */
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }
    else
    {
      Serial.println(input);
      /* The code snippet is fetching values from the `doc` JSON document. */
      // Fetch values.
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
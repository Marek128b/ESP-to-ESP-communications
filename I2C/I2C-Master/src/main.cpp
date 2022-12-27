// Wire Master Writer
// by Gutierrez PS <https://github.com/gutierrezps>
// ESP32 I2C slave library: <https://github.com/gutierrezps/ESP32_I2C_Slave>
// based on the example by Nicholas Zambetti <http://www.zambetti.com>

// Demonstrates use of the Wire and WirePacker libraries.
// Writes data to an ESP32 I2C/TWI slave device that
// uses ESP32 I2C Slave library.
// Refer to the "slave_receiver" example for use with this

#include <Arduino.h>
#include <Wire.h>
#include <WirePacker.h>

#include <ArduinoJson.h>

#define SDA_PIN 21
#define SCL_PIN 22
#define I2C_SLAVE_ADDR 0x04

DynamicJsonDocument doc(200);
void setup()
{
    // Add values in the document
    doc["sensor"] = "gps";
    doc["time"] = 1351824120;

    // Add an array.
    JsonArray data = doc.createNestedArray("data");
    data.add(48.756080);
    data.add(2.302038);

    Serial.begin(115200);         // start serial for output
    Wire.begin(SDA_PIN, SCL_PIN); // join i2c bus
}

void loop()
{
    static unsigned long lastWireTransmit = 0;
    static int count = 0;

    // send data to WireSlave device every 1000 ms
    if (millis() - lastWireTransmit > 1000)
    {
        doc["time"] = count;
        count++;
        
        String outData;
        serializeJson(doc, outData);

        // first create a WirePacker that will assemble a packet
        WirePacker packer;

        // then add data the same way as you would with Wire
        for (int i = 0; i < outData.length(); i++)
        {
            char currentChar = outData[i];
            packer.write(currentChar);
        }
        packer.write(0);

        // after adding all data you want to send, close the packet
        packer.end();

        Serial.printf("Sending to 0x%.2X:", I2C_SLAVE_ADDR);
        Serial.println(outData);

        // now transmit the packed data
        Wire.beginTransmission(I2C_SLAVE_ADDR);
        while (packer.available())
        { // write every packet byte
            int data = packer.read();
            Wire.write(data);
        }
        Wire.endTransmission(); // stop transmitting

        lastWireTransmit = millis();
    }
}
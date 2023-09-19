#include <Arduino.h>
#include <credentials.h> //WiFi username and passwords saved
#include <WiFi.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>

// Update these with values suitable for your network.
const char *mqtt_username = "htl-IoT";
const char *mqtt_password = "iot..2015";
const char *mqtt_server = "iotmqtt.htl-klu.at";

#define DATA_TOPIC "htl/3ahel/marek/data"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsgMillis = 0;
char msg[200];
int count = 0;

DynamicJsonDocument doc(200);
// ##########################################################################################################################################
// METHODES
void setup_wifi()
{
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// #########################################################################################################################################
// SETUP
void setup()
{
  // Add values in the document
  doc["sensor"] = "gps";
  doc["time"] = 1351824120;

  // Add an array.
  JsonArray data = doc.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}
// #########################################################################################################################################
// LOOP
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();

  if (millis() - lastMsgMillis > 2000)
  {
    ++count;
    doc["time"] = count;
    
    serializeJson(doc, msg);

    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(DATA_TOPIC, msg);

    lastMsgMillis = millis();
  }
}
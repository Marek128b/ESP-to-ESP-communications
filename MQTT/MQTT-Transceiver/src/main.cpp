#include <Arduino.h>
#include <credentials.h> //WiFi username and passwords saved
#include <WiFi.h>
#include <PubSubClient.h>

#include <ArduinoJson.h>

// Update these with values suitable for your network.
const char *mqtt_username = "htl-IoT";
const char *mqtt_password = "iot..2015";
const char *mqtt_server = "iotmqtt.htl-klu.at";

#define DATA_TOPIC_RX "htl/3ahel/marek/data<-"
#define DATA_TOPIC_TX "htl/3ahel/marek/data->"

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsgMillis = 0;
char msg[200];
int count = 0;

DynamicJsonDocument doc_tx(200);
StaticJsonDocument<200> doc_rx;
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

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  String inDataString = "";
  for (int i = 0; i < length; i++)
  {
    inDataString += (char)payload[i];
  }
  Serial.println(inDataString);

  // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc_rx, inDataString);

  // Test if parsing succeeds.
  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  const char *sensor = doc_rx["sensor"];
  long time = doc_rx["time"];
  double latitude = doc_rx["data"][0];
  double longitude = doc_rx["data"][1];

  // Print values.
  Serial.println(sensor);
  Serial.println(time);
  Serial.println(latitude, 6);
  Serial.println(longitude, 6);
  Serial.println();
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
      // ... and resubscribe
      client.subscribe(DATA_TOPIC_RX);
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
  doc_tx["sensor"] = "gps";
  doc_tx["time"] = 1351824120;

  // Add an array.
  JsonArray data = doc_tx.createNestedArray("data");
  data.add(48.756080);
  data.add(2.302038);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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
    doc_tx["time"] = count;

    serializeJson(doc_tx, msg);

    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish(DATA_TOPIC_TX, msg);

    lastMsgMillis = millis();
  }
}
#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#include <esp_now.h>
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

// REPLACE WITH YOUR RECEIVER MAC Address 78:21:84:7A:02:14
uint8_t broadcastAddress[] = {0x78, 0x21, 0x84, 0x7A, 0x02, 0x14};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message
{
  char a[32];
  int b;
  float c;
  bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
  // Init Serial Monitor
  Serial.begin(115200);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  // Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop()
{
  // Set values to send
  strcpy(myData.a, "THIS IS A CHAR");
  myData.b = random(1, 20);
  myData.c = 1.2;
  myData.d = !myData.d; // toggles boolean data every time send

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myData, sizeof(myData));

  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
    Serial.print("Int: ");
    Serial.println(myData.b); // Prints the random int data send
    Serial.print("Bool: ");
    Serial.println(myData.d); // Prints the boolean data send
  }
  else
  {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
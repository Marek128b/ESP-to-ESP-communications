#include <Arduino.h>

#include <WiFi.h>
#include <esp_now.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x58, 0xbf, 0x25, 0x33, 0x26, 0xf4};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message_tx
{
  char a[32];
  int b;
  float c;
  bool d;
} struct_message_tx;

// Create a struct_message_tx called myDataTX
struct_message_tx myDataTX;

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message_rx
{
  char a[32];
  int b;
  float c;
  bool d;
} struct_message_rx;

// Create a struct_message_rx called myData
struct_message_rx myDataRX;


esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myDataRX, incomingData, sizeof(myDataRX));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myDataRX.a);
  Serial.print("Int: ");
  Serial.println(myDataRX.b);
  Serial.print("Float: ");
  Serial.println(myDataRX.c);
  Serial.print("Bool: ");
  Serial.println(myDataRX.d);
  Serial.println();
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

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
  // Set values to send
  strcpy(myDataTX.a, "THIS IS A CHAR");
  myDataTX.b = random(1, 20);
  myDataTX.c = 1.2;
  myDataTX.d = !myDataTX.d; // toggles boolean data every time send

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&myDataTX, sizeof(myDataTX));

  if (result == ESP_OK)
  {
    Serial.println("Sent with success");
    Serial.print("Int: ");
    Serial.println(myDataTX.b); // Prints the random int data send
    Serial.print("Bool: ");
    Serial.println(myDataTX.d); // Prints the boolean data send
  }
  else
  {
    Serial.println("Error sending the data");
  }
  delay(2000);
}
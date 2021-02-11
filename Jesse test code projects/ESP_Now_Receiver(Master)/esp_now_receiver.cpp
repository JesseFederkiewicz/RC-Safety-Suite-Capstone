// 
// 
// 

#include "esp_now_receiver.h"
#include "esp_now_receiver.h"
#include <esp_now.h>
#include <WiFi.h>

//Data to send
typedef struct dbData {
	int a;
	int s;
	int t;
} dbData;

// Create a struct_message called myData
dbData receivedData;

// callback function that will be executed when data is received
void DataReceived(const uint8_t* mac, const uint8_t* data, int len) {
	Serial.println("Data Received:");
	memcpy(&receivedData, data, sizeof(receivedData));
	Serial.print("Angle: ");
	Serial.println(receivedData.a);
	Serial.print("Speed: ");
	Serial.println(receivedData.s);
	Serial.print("TimeStamp: ");
	Serial.println(receivedData.t);
}

void Main() 
{
	// Initialize Serial Monitor
	Serial.begin(115200);

	// Set device as a Wi-Fi Station
	WiFi.mode(WIFI_AP_STA);

	// Init ESP-NOW
	if (esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
		return;
	}

	Serial.println("Done esp init");

	//char* jesseSsid = "Cappy";
	//char* jessePass = "ThisIs@nAdequateP@ss123";

	//WiFi.begin(jesseSsid, jessePass);

	//while (WiFi.status() != WL_CONNECTED);

	//Serial.println("Wifi Connected");

	// Once ESPNow is successfully Init, we will register for recv CB to
	// get recv packer info
	esp_now_register_recv_cb(DataReceived);
	Serial.println("Callback registered");
}

/*
 Name:		ESP_NOW_Wireless_Slave_Requests.ino
 Created:	2/10/2021 5:36:00 PM
 Author:	Jesse Federkiewicz
*/

#include "SenderEspNow.h"
//#include <esp_now.h>
//#include <WiFi.h>

//uint8_t carMac[] = { 0x3C, 0x61, 0x05, 0x3E, 0xEE, 0xF4 };

// the setup function runs once when you press reset or power the board
void setup() {
	Main();

	//// Init Serial Monitor
	//Serial.begin(115200);

	//// Set device as a Wi-Fi Station
	//WiFi.mode(WIFI_AP_STA);

	//// Init ESP-NOW
	//if (esp_now_init() != ESP_OK) {
	//	Serial.println("Error initializing ESP-NOW");
	//	return;
	//}
	//char* jesseSsid = "Cappy";
	//char* jessePass = "ThisIs@nAdequateP@ss123";

	//WiFi.begin(jesseSsid, jessePass);

	//while (WiFi.status() != WL_CONNECTED);

	//Serial.println("Connected to WiFi");

	//// Once ESPNow is successfully Init, we will register for Send CB to
	//// get the status of Trasnmitted packet
	////esp_now_register_send_cb(OnDataSent);

	//// Register peer
	//esp_now_peer_info_t peerInfo;
	//memcpy(peerInfo.peer_addr, carMac, 6);
	//peerInfo.channel = 0;
	//peerInfo.encrypt = false;

	//// Add peer        
	//if (esp_now_add_peer(&peerInfo) != ESP_OK) {
	//	Serial.println("Failed to add peer");
	//	return;
	//}

	//Serial.println("Connected to Partner");
}

// the loop function runs over and over again until power down or reset
void loop() {
	return;
}


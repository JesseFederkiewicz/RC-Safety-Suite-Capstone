///////////////////////////////////////////////////////////////////////////////////////
// File:        WifiCode.cpp
// Author:      Tim Hachey
// Description: first time trying to get wifi running on the esp32
///////////////////////////////////////////////////////////////////////////////////////
#include "WifiCode.h"
#include "esp_wifi.h"
#include "HardwareSerial.h"
#include "WiFi.h"
#include "HTTPClient.h"

void Main() {

	char* ssid = "hachey wifi";
	const char* password = "38hachey";
	const char* serverName = "";

	Serial.begin(115200);
	WiFi.begin(ssid, password);


	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println();

	Serial.print("Connected, IP address: ");
	Serial.println(WiFi.localIP());
	

	for (;;) {
		
	}
}
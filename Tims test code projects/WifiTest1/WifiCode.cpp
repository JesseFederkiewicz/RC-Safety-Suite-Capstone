///////////////////////////////////////////////////////////////////////////////////////
// File:        WifiCode.cpp
// Author:      Tim Hachey
// Description: first time trying to get wifi running on the esp32
//				We have wireless communication to the webservice/Database
///////////////////////////////////////////////////////////////////////////////////////
#include "WifiCode.h"
#include "esp_wifi.h"
#include "HardwareSerial.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"
#include "SPI.h"

void Main() {
	char* ssid = "hachey wifi";
	const char* password = "38hachey";
	const char* webService = "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php";
	const char* postArgs = "action=GrabXYTimeStamp&carID=1";
	const char* server = "thor.net.nait.ca";


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

	HTTPClient http;
	WiFiClient client;

	for (;;) {
		if (WiFi.status() == WL_CONNECTED) {				

			//Your Domain name with URL path or IP address with path
			if(!http.connected())
				http.begin(webService);

			http.addHeader("Content-Type", "application/x-www-form-urlencoded");
			int httpResponseCode = http.POST(postArgs);

			//http.addHeader("Content-Type", "application/json");
			//int httpResponseCode = http.POST("{\"action\":\"GrabXYTimeStamp\",\"carID\":\"1\"}");

			if (httpResponseCode > 0) 
			{
				Serial.println("HTTP Response code: ");
				Serial.println(httpResponseCode);

				String payload = http.getString();	
				delay(1);
				Serial.println("Payload: ");
				JSONVar jason = JSON.parse(payload);
				Serial.println(jason["xCoord"]);
				Serial.println(jason["yCoord"]);
				Serial.println(jason["timeStamp"]);
			}
			else 
			{
				Serial.println("BAD RESPONSE");		
			}

			Serial.println("\nEND");

			//http.end();	
		}
	}
}
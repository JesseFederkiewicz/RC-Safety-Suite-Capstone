#include "SenderEspNow.h"
#include <esp_now.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "Arduino_JSON.h"

//Data to send
typedef struct dbData {
	int a;
	int s;
	int t;
} dbData;

// Struct to pack up contents into
dbData sendData;

HTTPClient _mainThread;
HTTPClient _secondThread;

uint8_t carMac[] = { 0x3C, 0x61, 0x05, 0x3E, 0xEE, 0xF4 };


void GrabData(bool isMainThread)
{
	String payload = "";

	int httpCode = -1;

	if (isMainThread)
	{
		_mainThread.addHeader("Content-Type", "application/x-www-form-urlencoded");
		httpCode = _mainThread.POST("action=GrabWebToCar&carID=1");
	}
	else
	{
		_secondThread.addHeader("Content-Type", "application/x-www-form-urlencoded");
		httpCode = _secondThread.POST("action=GrabWebToCar&carID=1");
	}

	if (httpCode == HTTP_CODE_OK)
	{
		// create buffer for read
		uint8_t buff[40] = { 0 };

		WiFiClient* stream;

		if (isMainThread)
			stream = _mainThread.getStreamPtr();
		else
			stream = _secondThread.getStreamPtr();

		// get available data size
		size_t size = stream->available();

		if (size) {
			// read up to 50 byte
			uint charPos = stream->readBytes(buff, size);

			bool inPayload = false;

			for (int i = 0; i < charPos; i++)
			{
				if (buff[i] == '{' || inPayload)
				{
					inPayload = true;

					payload += (char)buff[i];

					if (buff[i] == '}')
					{
						//exit conditions
						i = charPos;
					}
				}
			}
		}
		//Parse response into our boy jason
		JSONVar jason = JSON.parse(payload);


		//true || for testing as we might not be controlling db


		//Get data if timestamp has changed
		if (true || sendData.t != atoi(jason["t"]) && atoi(jason["t"]) != 0)
		{
			sendData.s = atoi(jason["a"]);
			sendData.a = atoi(jason["s"]);
			sendData.t = atoi(jason["t"]);

			Serial.println(sendData.s);
			Serial.println(sendData.a);
			Serial.println(sendData.t);
			esp_now_send(carMac, (uint8_t*)&sendData, sizeof(sendData));
		}

	}
}

void Core0Grab(void* param)
{
	Serial.println("START second Core");
	_secondThread.begin("https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/webservice.php");

	for (;;)
	{
		Serial.println("LOOPING second Core");
		GrabData(false);
	}
}


void OnDataSent(const uint8_t* mac_addr, esp_now_send_status_t status) {
	Serial.print("\r\nLast Packet Send Status:\t");
	Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void Main()
{
	// Init Serial Monitor
	Serial.begin(115200);

	// Set device as a Wi-Fi Station
	WiFi.mode(WIFI_AP_STA);

	// Init ESP-NOW
	if (esp_now_init() != ESP_OK) {
		Serial.println("Error initializing ESP-NOW");
		return;
	}

	// get the status of Trasnmitted packet
	esp_now_register_send_cb(OnDataSent);

	//char* jesseSsid = "Cappy";
	//char* jessePass = "ThisIs@nAdequateP@ss123";
	//WiFi.begin(jesseSsid, jessePass);

	char* timSsid = "tims wifi";
	char* timPass = "whatpassword";
	WiFi.begin(timSsid, timPass);

	while (WiFi.status() != WL_CONNECTED);

	Serial.println("Connected to WiFi");

	// Register peer
	esp_now_peer_info_t peerInfo;
	memcpy(peerInfo.peer_addr, carMac, 6);
	peerInfo.channel = 0;
	peerInfo.encrypt = false;

	// Add peer        
	if (esp_now_add_peer(&peerInfo) != ESP_OK) {
		Serial.println("Failed to add peer");
		return;
	}

	sendData.a, 0;
	sendData.s = 0;
	sendData.t = 1;

	// assign loop function for core 0
	TaskHandle_t core0Task; // task handle for core 0 task
	xTaskCreatePinnedToCore(
		Core0Grab,   /* Function to run on core 0*/
		"core0Task", /* Name of the task */
		10000,       /* Stack size in words */
		NULL,        /* Task input parameter */
		0,           /* Priority of the task */
		&core0Task,  /* Task handle. */
		0);          /* Core where the task should run */

	Serial.println("Partner Added");


	_mainThread.begin("https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/webservice.php");

	for (;;)
	{
		GrabData(true);
		Serial.println("LOOPING FIRST Core");
	}
}
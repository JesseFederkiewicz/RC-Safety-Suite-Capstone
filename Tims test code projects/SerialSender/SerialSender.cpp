// 
// 
// 

#include "SerialSender.h"
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_Json.h>

HTTPClient _mainThread;
HTTPClient _secondThread;

int _lastTime = 0;

String GrabData(bool isMainThread)
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
						JSONVar jason = JSON.parse(payload);

						int tempStamp = atoi(jason["t"]);

						// only pass newer timestamps
						if (tempStamp > _lastTime || (_lastTime - tempStamp > 5000))
						{
							_lastTime = tempStamp;

							return payload;
						}

						else
							return "outdated";
					}
				}
			}
		}
	}
	return "badcode";
}

void SendPayload(String payload) 
{
	if (payload != "outdated" && payload != "badcode" && Serial2.availableForWrite())
		Serial2.println(payload);
}

void Core0Grab(void* param)
{
	Serial2.println("START second Core");
	_secondThread.begin("https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/webservice.php");

	for (;;)
	{
		String payload = GrabData(false);
		SendPayload(payload);		
	}
}

void Main()
{
	// Init Serial2 Monitor
	Serial.begin(115200);
	Serial2.begin(115200);

	//char* jesseSsid = "Cappy";
	//char* jessePass = "ThisIs@nAdequateP@ss123";
	//WiFi.begin(jesseSsid, jessePass);

	char* timSsid = "tims wifi";
	char* timPass = "whatpassword";
	WiFi.begin(timSsid, timPass);

	Serial.println("Connecting");
	while (WiFi.status() != WL_CONNECTED) {}
	Serial.println("Connected");

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


	delay(400);

	_mainThread.begin("https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/webservice.php");

	for (;;)
	{
		String payload = GrabData(true);
		Serial.println(payload);
		SendPayload(payload);
	}
}
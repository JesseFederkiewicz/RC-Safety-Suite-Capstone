// 
// code for the slave board that sends post requests to the database and commands to the master board
// 

#include "SerialSender.h"
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_Json.h>
#include <mutex>

HTTPClient _mainThread;
HTTPClient _secondThread;

String serverURL = "https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/webservice.php";

bool _mainCoreSending = false;
bool _secondCoreSending = true;
int _lastTime = 0;
int _outDatedCounter = 0;
int _badCodeCounter = 0;

// in the event of bad return data from the web server this will be sent to the master board to stop the car
#define STOPCOMMAND "{\"a\":\"0\",\"s\":\"0\",\"t\":\"" + String(_lastTime) + "\",\"tc\":\"0\",\"abs\":\"0\"}!"

// values to send up to the database in post request
int FL_RPM = 0;
int FR_RPM = 0;
int BL_RPM = 0;
int BR_RPM = 0;
int GSP = 0;
int TC = 0;
int ABS = 0;
int BIP = 0;

// global string for holding the data received from the car 
//  to posted to the database, will already be in the right format
String _postString = "";

std::mutex myMutex; // used for locking the _postString so it doesnt crash when both cores read/set it

String GrabData(bool isMainThread)
{
	String payload = "";

	int httpCode = -1;

	if (isMainThread)
	{
		_mainThread.addHeader("Content-Type", "application/x-www-form-urlencoded");
		httpCode = _mainThread.POST(_postString);
	}
	else
	{
		_secondThread.addHeader("Content-Type", "application/x-www-form-urlencoded");
		httpCode = _secondThread.POST(_postString);
	}
	if (httpCode == HTTP_CODE_OK)
	{
		// create buffer for read
		uint8_t buff[75] = { 0 };

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

					//Check if a bad data object was returned
					if ((char)buff[i] == 'd')
						return "badcode";

					payload += (char)buff[i];

					if (buff[i] == '}')
					{
						Serial.println(payload);

						////Check if a bad data object was returned
						//if (payload.indexOf("Failed") > 0)
						//	return;

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

void SendReceiveSerial(String payload)
{
	// read data from car
	int serialByteSize = Serial1.available();
	if (serialByteSize)
	{
		//critical section
		myMutex.lock();
		_postString = Serial1.readStringUntil('!');
		Serial1.flush();
		myMutex.unlock();
	}
	//Serial.println(payload);
	//Serial.println(_postString);

	// send payload to car
	if (Serial2.availableForWrite()) {

		// send stop command on bad payload
		if (payload == "badcode") {
			_badCodeCounter++;

			if (_badCodeCounter == 2) {
				Serial2.print(STOPCOMMAND);
				_badCodeCounter = 0;
			}
			return;
		}
		// send stop command on too many outdateds
		if (payload == "outdated") {

			_outDatedCounter++;

			if (_outDatedCounter == 3) {
				Serial2.print(STOPCOMMAND);
				_outDatedCounter = 0;
			}
			return;
		}
		// send real payload if we make it past returns
		Serial2.print(payload + "!");
	}
}

void Core0Loop(void* param)
{
	_secondThread.begin(serverURL);

	for (;;)
	{
		if (_mainCoreSending)
			delay(200);

		_secondCoreSending = true;
		String payload = GrabData(false);
		_secondCoreSending = false;

		SendReceiveSerial(payload);
	}
}

void Main()
{
	Serial.begin(115200);
	Serial2.begin(115200);
	Serial1.begin(115200, SERIAL_8N1, 16);

	const char* timsssid = "_starLink";
	const char* timspassword = "whatpassword";
	const char* jesseSsid = "Cappy";
	const char* jessePass = "ThisIs@nAdequateP@ss123";

	WiFi.begin(jesseSsid, jessePass);

	int connectionCounter = 0;

	while (WiFi.status() != WL_CONNECTED) {
		delay(250);
		connectionCounter++;

		if (connectionCounter > 20)
		{
			WiFi.disconnect();
			WiFi.begin(timsssid, timspassword);

			while (WiFi.status() != WL_CONNECTED) {
				delay(250);
			}
		}
	}

	// assign loop function for core 0
	TaskHandle_t core0Task; // task handle for core 0 task
	xTaskCreatePinnedToCore(
		Core0Loop,   /* Function to run on core 0*/
		"core0Task", /* Name of the task */
		10000,       /* Stack size in words */
		NULL,        /* Task input parameter */
		0,           /* Priority of the task */
		&core0Task,  /* Task handle. */
		0);          /* Core where the task should run */

	delay(450);

	_mainThread.begin(serverURL);

	for (;;)
	{
		if (_secondCoreSending)
			delay(200);

		_mainCoreSending = true;
		String payload = GrabData(true);
		_mainCoreSending = false;

		SendReceiveSerial(payload);
	}
}
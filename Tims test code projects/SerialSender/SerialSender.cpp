////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File:		SerialSender.cpp
// Authors:     Tim Hachey, Jesse Federkiewicz 
// Description: secondary board: sends commands and recieves data from the master board via serial,
//				sends and recieves data to/from the database via Wifi connection.
//				runs http clients and serial comms on two threads to reduce latency when driving.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "SerialSender.h"
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_Json.h>
#include <mutex>

// client object for each thread
HTTPClient _mainThread;
HTTPClient _secondThread;

String serverURL = "https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/webservice.php";

// used to off set the http clients/threads so they work in a staggered fashion
bool _mainCoreSending = false;
bool _secondCoreSending = true;

int _lastTime = 0;		  // last received timestamp is stored to check against on the next receive
int _outDatedCounter = 0; // counts the number of outdated timestamps received
int _badCodeCounter = 0;  // counts the number of bad codes received from the http client
const int OUTDATEDLIMIT = 3;  
const int BADCODELIMIT = 2;  

// in the event of bad return data from the web server this will be sent to the master board to stop the car
#define STOPCOMMAND "{\"a\":\"0\",\"s\":\"0\",\"t\":\"" + String(_lastTime) + "\",\"tc\":\"0\",\"abs\":\"0\"}!"

String _postString = ""; /* string for holding the data received from the car, to be posted to the database,
							 will be received from the master board in the right format*/

std::mutex myMutex; // used for locking the _postString so it doesnt crash when both cores read/set it

// sends a post request to the database containing carID, rpm data and others.
// extracts the incoming payload, filters out old timestamps, returns the payload or an error code
String GrabData(bool isMainThread)
{
	String payload = ""; // will be returned if valid payload is received
	int httpCode = -1;   // http code to check if post request was valid

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
<<<<<<< Updated upstream
	{
		// create buffer for read
		uint8_t buff[75] = { 0 };
=======
	{		
		uint8_t buff[65] = { 0 }; // buffer to read into
>>>>>>> Stashed changes

		WiFiClient* stream;
		if (isMainThread)
			stream = _mainThread.getStreamPtr();
		else
			stream = _secondThread.getStreamPtr();

		// get payload from server if available
		size_t size = stream->available();
		if (size) 
		{
			bool inPayload = false; // used to filter the first char
			uint charPos = stream->readBytes(buff, size);

			// loop builds the payload one char at a time
			for (int i = 0; i < charPos; i++)
			{
				// filter bad data: if 1st char isnt {
				if (buff[i] == '{' || inPayload)
				{
					inPayload = true;

					//Check if a bad data object was returned: payload should never contain d
					if ((char)buff[i] == 'd')
						return "badcode";

					// add char to payload
					payload += (char)buff[i];

					// end of payload reached
					if (buff[i] == '}')
					{
<<<<<<< Updated upstream
						Serial.println(payload);

						////Check if a bad data object was returned
						//if (payload.indexOf("Failed") > 0)
						//	return;

=======
						// parse the timestamp out of payload
>>>>>>> Stashed changes
						JSONVar jason = JSON.parse(payload);
						int tempStamp = atoi(jason["t"]);

						// only pass payloads with newer timestamps
						if (tempStamp > _lastTime || (_lastTime - tempStamp > 5000))
						{
							// good payload received
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

// sends commands to the master board via serial and 
//  receives data to be posted to the database from the master board
void SendReceiveSerial(String payload)
{
	// read data from master if available
	int serialByteSize = Serial1.available();
	if (serialByteSize)
	{
		//critical section, lock to avoid cross thread exceptions
		myMutex.lock();
		_postString = Serial1.readStringUntil('!');
		Serial1.flush();
		myMutex.unlock();
	}
	//Serial.println(payload);
	//Serial.println(_postString);

	// send payload to car
	if (Serial2.availableForWrite()) {

		// send stop command if too many bad payloads
		if (payload == "badcode") {
			_badCodeCounter++;

			if (_badCodeCounter >= BADCODELIMIT) {
				Serial2.print(STOPCOMMAND);
				_badCodeCounter = 0;
			}
			return;
		}
		// send stop command on too many outdateds
		if (payload == "outdated") {
			_outDatedCounter++;

			if (_outDatedCounter >= OUTDATEDLIMIT) {
				Serial2.print(STOPCOMMAND);
				_outDatedCounter = 0;
			}
			return;
		}
		// send real payload if we make it past returns
		Serial2.print(payload + "!");
	}
}

// main loop function for core 0
void Core0Loop(void* param)
{
	// initialize http client
	_secondThread.begin(serverURL);

	for (;;)
	{
		// stagger threads
		if (_mainCoreSending)
			delay(200);

		// get data from database
		_secondCoreSending = true;
		String payload = GrabData(false);
		_secondCoreSending = false;

		// send/get payload/data to/from master board
		SendReceiveSerial(payload);
	}
}

// main initilizations and loop function for main core thread
void Main()
{
	//Serial.begin(115200); // used for debugging
	Serial2.begin(115200);                 // serial channel for sending to master
	Serial1.begin(115200, SERIAL_8N1, 16); // serial channel for recieving from master

	// wifi credentials
	const char* timsssid = "_starLink";
	const char* timspassword = "whatpassword";
	const char* jesseSsid = "Cappy";
	const char* jessePass = "ThisIs@nAdequateP@ss123";

	// connect to first set of credentials
	WiFi.begin(jesseSsid, jessePass);
	int connectionCounter = 0;

	while (WiFi.status() != WL_CONNECTED) {
		delay(250);
		connectionCounter++;

		// if no connection, try next credentials
		if (connectionCounter > 20)
		{
			WiFi.disconnect();
			WiFi.begin(timsssid, timspassword);

			while (WiFi.status() != WL_CONNECTED) {
				delay(250);
			}
		}
	}

	// initialize/assign loop function for core 0
	TaskHandle_t core0Task; // task handle for core 0 task
	xTaskCreatePinnedToCore(
		Core0Loop,   /* Function to run on core 0*/
		"core0Task", /* Name of the task */
		10000,       /* Stack size in words */
		NULL,        /* Task input parameter */
		0,           /* Priority of the task */
		&core0Task,  /* Task handle. */
		0);          /* Core where the task should run */

	// staggers threads of the start
	delay(450);

	// initialze main thread http client
	_mainThread.begin(serverURL);

	for (;;)
	{
		// stagger threads
		if (_secondCoreSending)
			delay(200);

		// get data from database
		_mainCoreSending = true;
		String payload = GrabData(true);
		_mainCoreSending = false;

		// send/get payload/data to/from master board
		SendReceiveSerial(payload);
	}
}
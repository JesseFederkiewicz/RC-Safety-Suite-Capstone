// 
// code for the slave board that sends post requests to the database and commands to the master board
// 

#include "SerialSender.h"
#include <HardwareSerial.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_Json.h>

HTTPClient _mainThread;
HTTPClient _secondThread;

String serverURL = "https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/webservice.php";

bool _mainCoreSending = false;
bool _secondCoreSending = true;
int _lastTime = 0;

// values to send up to the database in post request
int FL_RPM = 0;
int FR_RPM = 0;
int BL_RPM = 0;
int BR_RPM = 0;
int GSP = 0;
int TC = 0;
int ABS = 0;
int BIP = 0;

String GrabData(bool isMainThread)
{
	String payload = "";

	int httpCode = -1;

	// crashes program:
	/*char *buff;
	sprintf("carID=1&GSP=%d&FL_RPM=%d&FR_RPM=%d&BL_RPM=%d&BR_RPM=%d&TC=%d&ABS=%d&BIP=%d", buff, GSP, FL_RPM, FR_RPM, BL_RPM, TC, ABS, BIP);*/

	// build string for post request
	String PostString = "carID=1";	
	PostString +="&GSP=0";//+ GSP;       // need string interpolation or some such
	PostString +="&FL_RPM=0";//+ FL_RPM;
	PostString +="&FR_RPM=0";//+ FR_RPM;
	PostString +="&BL_RPM=0";//+ BL_RPM;
	PostString +="&BR_RPM=0";//+ BR_RPM;
	PostString +="&TC=0";//+ TC;
	PostString +="&ABS=0";//+ ABS;
	PostString += "&BIP=0";// +BIP;

	Serial.println(PostString);

	if (isMainThread)
	{
		_mainThread.addHeader("Content-Type", "application/x-www-form-urlencoded");
		httpCode = _mainThread.POST(PostString);
	}
	else
	{
		_secondThread.addHeader("Content-Type", "application/x-www-form-urlencoded");
		httpCode = _secondThread.POST(PostString);
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
	if (Serial2.availableForWrite())
	{
		//Serial.println(payload);
		Serial2.println(payload + "!");
	}
}

void Core0Grab(void* param)
{
	Serial2.println("START second Core");
	_secondThread.begin(serverURL);

	for (;;)
	{
		if (_mainCoreSending)
			delay(200);

		_secondCoreSending = true;
		String payload = GrabData(false);
		_secondCoreSending = false;
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

	//Serial.println("Connecting");

	const char* timsHotssid = "tims wifi";
	const char* timsHotpassword = "whatpassword";
	const char* timsShitternet = "hachey wifi 2.4 GHz";
	const char* timsShitternetPass = "38hachey";
	const char* jesseSsid = "Cappy";
	const char* jessePass = "ThisIs@nAdequateP@ss123";

	WiFi.begin(jesseSsid, jessePass);

	//char* timssid = "hachey wifi 2.4 GHz";
	//const char* timpassword = "38hachey";
	//WiFi.begin(timssid, timpassword);
	int connectionCounter = 0;

	//Serial.println("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(250);
		connectionCounter++;

		if (connectionCounter > 20)
		{
			WiFi.disconnect();
			WiFi.begin(timsShitternet, timsShitternetPass);
			//Serial.println("REConnecting");
			while (WiFi.status() != WL_CONNECTED) {
				delay(250);
			}
		}
	}

	//Serial.println("Connected");

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


	delay(450);

	_mainThread.begin(serverURL);
	int i = 0;
	for (;;)
	{
		if (_secondCoreSending)
			delay(200);

		_mainCoreSending = true;
		String payload = GrabData(true);
		Serial.println(payload);
		_mainCoreSending = false;
		SendPayload(payload);
	}
}
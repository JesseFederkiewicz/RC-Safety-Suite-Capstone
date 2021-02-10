// 
// File: Wireless.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "Wireless.h"

HTTPClient client; // used in GrabData to get data from the database, initialized in InitWiFi()

void InitWiFi() 
{
	const char* webService = "https://coolstuffliveshere.com/Rc_Safety_Suite/Main_Web/webservice.php";
	//const char* webService = "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php";

	//char* jessessidHOT = "Unhackable II";
	//const char* jessepasswordHOT = "plsdontguess";
	//WiFi.begin(jessessidHOT, jessepasswordHOT);

	//char* jessessid = "Cappy";
	//const char* jessepassword = "ThisIs@nAdequateP@ss123";
	//WiFi.begin(jessessid, jessepassword);
		
	char* timsHotssid = "tims wifi";
	const char* timsHotpassword = "whatpassword";
	WiFi.begin(timsHotssid, timsHotpassword);

	//char* timssid = "hachey wifi 2.4 GHz";
	//const char* timpassword = "38hachey";
	//WiFi.begin(timssid, timpassword);

	//Serial.println("Connecting");
	//Serial.printf("status: %d", WiFi.status());
	while (WiFi.status() != WL_CONNECTED) {
		//delay(250);	
	}
	//Serial.println("Connected, IP address: ");
	//Serial.println(WiFi.localIP());

	client.begin(webService);
}

String GrabData()
{
	int _lossPings = 0;
	int httpCode = -1;
	String payload = "";

	client.addHeader("Content-Type", "application/x-www-form-urlencoded");
	httpCode = client.POST("action=GrabWebToCar&carID=1");

	// file found at server
	if (httpCode == HTTP_CODE_OK)
	{
		// create buffer for read
		uint8_t buff[40] = { 0 };

		WiFiClient* stream;

		// get tcp stream
		stream = client.getStreamPtr();

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
		////Parse response into our boy jason
		//JSONVar jason = JSON.parse(payload);

		////Get data if timestamp has changed
		//if (_timeStamp != atoi(jason["t"]) && atoi(jason["t"]) != 0)
		//{
		//	_intendedAngle = atoi(jason["a"]);
		//	_intendedSpeedPercent = atoi(jason["s"]);
		//	_timeStamp = atoi(jason["t"]);
		//	_lossPings = 0;

		//	//Serial.println(_intendedAngle);
		//	//Serial.println(_intendedSpeedPercent);
		//	//Serial.println(_timeStamp);
		//}
		else
		{
			_lossPings++;

			if (_lossPings > 2)
			{
				/*_intendedAngle = 0;
				_intendedSpeedPercent = 0;
				_timeStamp = 0;*/
				return "-1";
			}
		}		
		return payload;
	}
}
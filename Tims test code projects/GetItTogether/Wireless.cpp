// 
// 
// 

#include "Wireless.h"

HTTPClient client;
int _lossPings = 0;

void InitWiFi() 
{
	char* jessessidHOT = "Unhackable II";
	const char* jessepasswordHOT = "plsdontguess";
	char* jessessid = "Cappy";
	const char* jessepassword = "ThisIs@nAdequateP@ss123";
	char* timssid = "hachey wifi";
	const char* timpassword = "38hachey";
	const char* webService = "https://coolstuffliveshere.com/Hobby_Projects/Rc_Safety_Suite/Main%20Web/webservice.php";

	WiFi.begin(timssid, timpassword);

	//Serial.print("Connecting");

	while (WiFi.status() != WL_CONNECTED) {
		//delay(200);
		//Serial.print(".");
	}
	//Serial.println();
	//Serial.print("Connected, IP address: ");
	//Serial.println(WiFi.localIP());

	client.begin(webService);
}

String GrabData()
{
	int httpCode = -1;
	String payload = "";

	client.addHeader("Content-Type", "application/x-www-form-urlencoded");
	httpCode = client.POST("action=GrabWebToCar&carID=1");


	// file found at server
	if (httpCode == HTTP_CODE_OK)
	{
		// create buffer for read
		uint8_t buff[50] = { 0 };

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
				//Serial.println(buff[i]);
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
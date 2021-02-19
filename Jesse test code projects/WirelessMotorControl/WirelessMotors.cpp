///////////////////////////////////////////////////////////////////////////////////////
// File:        WirelessMotors.cpp
// Author:      Tim Hachey
// Description: now that we have wifi/database communication, 
//				lets put it together and control the motors from the web!
//
//				Lots of debugging, fix was heap space shown in this file
///////////////////////////////////////////////////////////////////////////////////////
#include "WirelessMotors.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"
#include "HardwareSerial.h"
#include "driver/mcpwm.h"
#include "esp32-hal.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "arduino.h"
#include "driver/pcnt.h"
#include <WiFi.h>

char* jessessid = "No Issac";
const char* jessepassword = "Really No Issac";
const char* webService = "thor.net...../webservice.php";

void Main()
{

	WiFi.begin(jessessid, jessepassword);

	while (WiFi.status() != WL_CONNECTED);
	
	for (;;)
	{
		HTTPClient client;

		client.begin(webService);

		if (WiFi.status() == WL_CONNECTED)
		{
			client.addHeader("Content-Type", "application/x-www-form-urlencoded");
			int httpCode = client.POST("action=GrabWebToCar&carID=2");

			// file found at server
			if (httpCode == HTTP_CODE_OK)
			{

				String returnStuff = client.getString();

				/*sample return

							{"a":"0","s":"0","t":"28335"}

				*/
			
				JSONVar jason = JSON.parse(returnStuff);
			}

		}
	}
	
}


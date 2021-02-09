// 
// 
// 

//#include "async_web.h"
#include <Bridge.h>
#include <WIFI.h>
#include <Process.h>
#include "A:\Program Files (x86)\Arduino\libraries\Bridge\src\HttpClient.h"


void Main()
{
	Bridge.begin();
	Serial.begin(115200);
	Serial.println("Serial Has begun");
	//Bridge.begin();
	//Bridge.begin();
	char* jessessidHOT = "Unhackable II";
	const char* jessepasswordHOT = "plsdontguess";
	char* jessessid = "Cappy";
	const char* jessepassword = "ThisIs@nAdequateP@ss123";
	char* timssid = "hachey wifi";
	const char* timpassword = "38hachey";
	const char* webService = "https://coolstuffliveshere.com/Hobby_Projects/Rc_Safety_Suite/Main%20Web/webservice.php";
	const char* server = "thor.net.nait.ca";

	WiFi.begin(jessessid, jessepassword);

	while (WiFi.status() != WL_CONNECTED) {
	}

	Serial.println("Wifi connected");

	HttpClient client;
	//client.setHeader("Content-Type: application/x-www-form-urlencoded");
	
	client.setHeader("Content-Type: application/x-www-form-urlencoded");

	char* data = "action=GrabWebToCar&carID=2";


	for (;;)
	{
		Serial.println("Before post");
		/*int postData = client.post(webService, data);*/
		client.postAsynchronously(webService, data);
		Serial.println("After post");
		Serial.println("Waiting");
		while (!client.ready()) {
			Serial.print(".");
			delay(1);
		}

		if (client.ready())
		{
			Serial.print("Async response:");
			while (client.available() > 1)
			{
				Serial.println(client.read());
			}

			//String s = client.readString();

		}
	}

	//while (client.available())
	//{
	//	char c = client.read();
	//	Serial.println(c);
	//}

	//Serial.println("End of program");

	//String payload = client.readString();
	//Serial.println("After ReadString");
	//Serial.println(payload);

	//for (;;)
	//{
	//	String payload = "";
	//	//int postData = client.post(webService, data);


	//	while (client.available() > 1)
	//	{
	//		//int c = client.read();
	//		//Serial.println(c);
	//		////Serial.println(client.available());
	//	}

	//	Serial.println("Past Availiable");

	//	//Serial.println("Post data: ");
	//	//Serial.println(postData);

	//	//int res = client.getResult();

	//	//Serial.println("GetResult");
	//	//Serial.println(res);

	//	//	//client.
	//	//

	//}
}


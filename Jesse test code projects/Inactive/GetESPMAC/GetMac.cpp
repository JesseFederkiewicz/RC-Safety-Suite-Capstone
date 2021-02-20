// 
// 
// 

#include "GetMac.h"
#include <WiFi.h>

void Main()
{
	Serial.begin(115200);
	WiFi.mode(WIFI_MODE_STA);
	Serial.println(WiFi.macAddress());
}
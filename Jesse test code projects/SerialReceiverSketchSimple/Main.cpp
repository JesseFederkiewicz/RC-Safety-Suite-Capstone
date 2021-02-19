// 
// 
// 

#include "Main.h"

void Main()
{
	while (Serial2.available()) {
		Serial.printf("%c", Serial2.read());
	}
}

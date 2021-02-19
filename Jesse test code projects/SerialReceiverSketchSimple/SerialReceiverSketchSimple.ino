/*
 Name:		SerialReceiverSketchSimple.ino
 Created:	2/18/2021 3:43:36 PM
 Author:	Jesse Federkiewicz
*/

// the setup function runs once when you press reset or power the board
#include "Main.h"
void setup() {
	Serial.begin(115200);
	Serial2.begin(115200);
}

// the loop function runs over and over again until power down or reset
void loop() {
	Main();
}

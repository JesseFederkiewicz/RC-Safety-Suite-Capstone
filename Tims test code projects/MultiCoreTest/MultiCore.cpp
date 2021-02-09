// 
// File:        MultiCore.cpp
// Author:      Tim Hachey
// Description: First test of using both cores of the esp32
//

#include "MultiCore.h"
#include "HardwareSerial.h"
#include "arduino.h"

void Core0Func(void* param) {
	for (;;) {
		Serial.printf("New thread on Core#: %d\n", xPortGetCoreID());
		delay(1000);
	}
}


void Main() {

	// lets muiltiCore this
	Serial.begin(115200);
	TaskHandle_t task1;

	xTaskCreatePinnedToCore(
		Core0Func, /* Function to implement the task */
		"task1",   /* Name of the task */
		10000,     /* Stack size in words */
		NULL,      /* Task input parameter */
		0,         /* Priority of the task */
		&task1,    /* Task handle. */
		0);        /* Core where the task should run */


	for (;;) 
	{
		delay(250);
		Serial.printf("Main thread on Core#: %d\n", xPortGetCoreID());
	}
}
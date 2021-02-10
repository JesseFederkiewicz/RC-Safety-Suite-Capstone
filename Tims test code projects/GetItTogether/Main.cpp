// 
// File: Main.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "Main.h"

// interrupt timer stuff:
volatile bool intFlag = false; // flag for use in main for actual code to run every interrupt interval
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // used for syncing main and isr, ignore this red squiggle, still works

// timer ISR, trips a flag to use in main
void IRAM_ATTR TimerInt()
{
	portENTER_CRITICAL_ISR(&timerMux);
	intFlag = true;
	portEXIT_CRITICAL_ISR(&timerMux);
}

// values for driving
int _intendedAngle = 0;
int _intendedSpeed = 0;
int _timeStamp = 1;

// Core 0 will be dedicated to getting data from database and 
//  updating the values used for driving/steering
void Core0Loop(void* param)
{
	for (;;)
	{
		if (WiFi.status() == WL_CONNECTED)
		{
			String payload = GrabData();

			if (payload == "-1" || payload == "" || payload == " ")
			{
				_intendedAngle = 0;
				_intendedSpeed = 0;
				_timeStamp = 0;
			}
			else
			{
				//Parse response into our boy jason
				JSONVar jason = JSON.parse(payload);

				//Get data if timestamp has changed
				int tempStamp = atoi(jason["t"]);

				if (_timeStamp != tempStamp && tempStamp != 0)
				{
					_intendedAngle = atoi(jason["a"]);
					_intendedSpeed = atoi(jason["s"]);
					_timeStamp = tempStamp;

					//Serial.println(_intendedAngle);
					//Serial.println(_intendedSpeed);
					//Serial.println(_timeStamp);
				}
			}
		}
		else {
			InitWiFi();
		}
	}
}

// Main runs on core 1
void Main()
{
	//Serial.begin(115200);

	// call all Inits
	InitWiFi();
	InitMotors();
	InitEncoders();
	TimerInterruptInit(TimerInt);

	// assign loop function for core 0
	TaskHandle_t core0Task; // task handle for core 0 task
	xTaskCreatePinnedToCore(
		Core0Loop,   /* Function to run on core 0*/
		"core0Task", /* Name of the task */
		10000,       /* Stack size in words */
		NULL,        /* Task input parameter */
		0,           /* Priority of the task */
		&core0Task,  /* Task handle. */
		0);          /* Core where the task should run */

	RPMS rpms;

	for (;;)
	{
		SimpleSteering(_intendedAngle, _intendedSpeed);

		if (intFlag)
		{
			rpms = GetRPMS();

			//Serial.println("RPMs:");
			//Serial.printf("LF: %f\n", rpms.FL_RPM);
			//Serial.printf("RF: %f\n", rpms.FR_RPM);
			//Serial.printf("LR: %f\n", rpms.BL_RPM);
			//Serial.printf("RR: %f\n", rpms.BR_RPM);

			portENTER_CRITICAL(&timerMux);
			intFlag = false;
			portEXIT_CRITICAL(&timerMux);
		}
	}
}
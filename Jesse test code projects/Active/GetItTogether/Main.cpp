// 
// File: Main.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 
// Current Progress: Ground Speed Sensors and RPM Complete -- that should be it for hardware!

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

void ReadSerialPayload()
{
	String payload = "";

	if (Serial2.available()) 
	{
		payload = Serial2.readStringUntil('!');

		if (payload.length() < 12) return;

		payload.trim();

		//Serial.println(payload);
		
		JSONVar jason = JSON.parse(payload);

		int tempStamp = atoi(jason["t"]);

		if (tempStamp > _timeStamp || (_timeStamp - tempStamp > 5000))
		{
			_intendedAngle = atoi(jason["a"]);
			_intendedSpeed = atoi(jason["s"]);
			_timeStamp = tempStamp;

			//Serial.println(_intendedAngle);
			//Serial.println(_intendedSpeed);
			//Serial.println(_timeStamp);
			//Serial.println();
		}
	}

	///

	///



	////Get data if timestamp has changed
	//int tempStamp = atoi(jason["t"]);

	//if (tempStamp > _timeStamp || (_timeStamp - tempStamp > 5000))
	//{
	//	_intendedAngle = atoi(jason["a"]);
	//	_intendedSpeed = atoi(jason["s"]);
	//	_timeStamp = tempStamp;

	//	Serial.println(_timeStamp);
	//	return;
	//}


	// read up to 40 byte
	//uint pos = Serial1.readBytes(buff, 40);

	//bool inPayload = false;

	//for (int i = 0; i < pos; i++)
	//{
	//	if (buff[i] == '{' || inPayload)
	//	{
	//		inPayload = true;

	//		payload += (char)buff[i];

	//		if (buff[i] == '}')
	//		{
	//			JSONVar jason = JSON.parse(payload);

	//			Serial.println(payload);

	//			//Get data if timestamp has changed
	//			int tempStamp = atoi(jason["t"]);

	//			if (tempStamp > _timeStamp || (_timeStamp - tempStamp > 5000))
	//			{
	//				_intendedAngle = atoi(jason["a"]);
	//				_intendedSpeed = atoi(jason["s"]);
	//				_timeStamp = tempStamp;
	//				
	//				//Serial.println(_intendedAngle);
	//				//Serial.println(_intendedSpeed);
	//				//Serial.println(_timeStamp);
	//				return;
	//			}
	//		}
	//	}
	//}


	//if (payload2 != "" && payload2 != " ")
	//{
	//	//Parse response into our boy jason
	//	JSONVar jason = JSON.parse(payload2);

	//	//Get data if timestamp has changed
	//	int tempStamp = atoi(jason["t"]);

	//	if (_timeStamp != tempStamp && tempStamp != 0)
	//	{
	//		_intendedAngle = atoi(jason["a"]);
	//		_intendedSpeed = atoi(jason["s"]);
	//		_timeStamp = tempStamp;

	//		//Serial.println(_intendedAngle);
	//		//Serial.println(_intendedSpeed);
	//		//Serial.println(_timeStamp);
	//	}
	//}
	//else {
	//	_intendedSpeed = 0;
	//}

	////String payload1 = Serial2.readStringUntil(0);
	////Serial.println(payload1);
	//if (payload1 != "" && payload2 != " ")
	//{
	//	//Parse response into our boy jason
	//	JSONVar jason = JSON.parse(payload1);

	//	//Get data if timestamp has changed
	//	int tempStamp = atoi(jason["t"]);

	//	if (_timeStamp != tempStamp && tempStamp != 0)
	//	{
	//		_intendedAngle = atoi(jason["a"]);
	//		_intendedSpeed = atoi(jason["s"]);
	//		_timeStamp = tempStamp;

	//		//Serial.println(_intendedAngle);
	//		//Serial.println(_intendedSpeed);
	//		//Serial.println(_timeStamp);
	//	}
	//}
	//else {
	//	_intendedSpeed = 0;
	//}
}

// Core 0 will be dedicated to getting data from database and 
//  updating the values used for driving/steering
void Core0Loop(void* param)
{
	for (;;)
	{
		ReadSerialPayload();

		//Here for now until more is done, if it spins too much guru error
		delay(10);
		//ReadSerialPayload();

		//if (WiFi.status() == WL_CONNECTED)
		//{
		//	String payload = GrabData();
		//	//Serial.println(payload);
		//	if (payload == "-1" || payload == "" || payload == " ")
		//	{
		//		_intendedAngle = 0;
		//		_intendedSpeed = 0;
		//		_timeStamp = 0;
		//	}
		//	else
		//	{
		//		//Parse response into our boy jason
		//		JSONVar jason = JSON.parse(payload);

		//		//Get data if timestamp has changed
		//		int tempStamp = atoi(jason["t"]);

		//		if (_timeStamp != tempStamp && tempStamp != 0)
		//		{
		//			_intendedAngle = atoi(jason["a"]);
		//			_intendedSpeed = atoi(jason["s"]);
		//			_timeStamp = tempStamp;

		//			//Serial.println(_intendedAngle);
		//			//Serial.println(_intendedSpeed);
		//			//Serial.println(_timeStamp);
		//		}
		//	}
		//}
		//else {
		//	InitWiFi();
		//}
	}
}

// Main runs on core 1
void Main()
{
	Serial.begin(115200);
	Serial2.begin(115200);
	//Serial2.begin(115200); 

	GroundSpeedSensorInit();

	// call all Inits
	// InitWiFi();
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
		//ReadSerialPayload();
		if (intFlag)
		{
			rpms = GetRPMS();

			portENTER_CRITICAL(&timerMux);
			intFlag = false;
			portEXIT_CRITICAL(&timerMux);

			//Serial.println("RPMS:");
			//Serial.println(rpms.FL_RPM);
			//Serial.println(rpms.FR_RPM);
			//Serial.println(rpms.BL_RPM);
			//Serial.println(rpms.BR_RPM);

			int16_t ret = GetGroundSpeedCounterVal();

			//Serial.println(ret);
			DrivingWithBrakesAndSteering(_intendedAngle, _intendedSpeed, rpms);
		}
		/*DrivingWithBrakesAndSteering(_intendedAngle, _intendedSpeed, rpms);*/
	}
}
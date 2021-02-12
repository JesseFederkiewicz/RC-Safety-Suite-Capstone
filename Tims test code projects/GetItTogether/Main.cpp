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

void ReadSerialPayload()
{
	// create buffer for read
	//char* buff = "";
	int i = 0;
	String payload = "";

	uint8_t c[40];
	char check = Serial1.read();
	//Serial.print(c);
	if (check == '{')
	{
		c[0] = '{';
		while (c[i] != '}' && c[i] >= '"' && c[i] <= '}')
		{
			c[i] = Serial1.read();
			payload += c[i];
			Serial.print(c[i]);
		}
	}

	for (int x = 0; x < 41; x++)
	{
		Serial.printf("%c", c[i]);
	}

	Serial.println();
	Serial.println();

	//if (Serial1.available())
	//{
	//	char c;
	//	while (c != '}')
	//	{
	//		c = (char)Serial1.read();
	//		payload += c;
	//		Serial.print(c);
	//	} 		

	//	JSONVar jason = JSON.parse(payload);
	//	Serial.println(payload);
	//	Serial.println("test");
	//}


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

	//String payload1 = Serial2.readStringUntil(0);
	//Serial.println(payload1);
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
		//delay(10);
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
	Serial1.begin(115200, 134217756, 5);
	//Serial2.begin(115200); 

	// call all Inits
	// InitWiFi();
	InitMotors();
	InitEncoders();
	TimerInterruptInit(TimerInt);

	//// assign loop function for core 0
	//TaskHandle_t core0Task; // task handle for core 0 task
	//xTaskCreatePinnedToCore(
	//	Core0Loop,   /* Function to run on core 0*/
	//	"core0Task", /* Name of the task */
	//	10000,       /* Stack size in words */
	//	NULL,        /* Task input parameter */
	//	0,           /* Priority of the task */
	//	&core0Task,  /* Task handle. */
	//	0);          /* Core where the task should run */

	RPMS rpms;

	for (;;)
	{
		ReadSerialPayload();
		if (intFlag)
		{
			rpms = GetRPMS();

			portENTER_CRITICAL(&timerMux);
			intFlag = false;
			portEXIT_CRITICAL(&timerMux);
		}
		DrivingWithBrakesAndSteering(_intendedAngle, _intendedSpeed, rpms);
	}
}
// 
// File: Main.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "Main.h"

// values for driving
int _intendedAngle = 0;
int _intendedSpeed = 0;
int _timeStamp = 1;
RPMS rpms = { 0,0,0,0,stopped,stopped,stopped,stopped };

// interrupt stuff:
volatile bool timerIntFlag = false;	// flag for use in main for actual code to run every interrupt interval
volatile bool encIntFlag = false;

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;   // used for syncing main and isr, ignore this red squiggle, still works
portMUX_TYPE encoderMux = portMUX_INITIALIZER_UNLOCKED;

// Quadrature Encoder Matrix courtesy of: https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf
Movement QEM[16] = { stopped,backward,forward,X,
					 forward,stopped,X,backward,
					 backward,X,stopped,forward,
					 X,forward,backward,stopped };

// values for determining the direction of wheel spin using the matrix above
int LFvalOld = 0;
int LFvalNew = 0;
int RFvalOld = 0;
int RFvalNew = 0;
int RRvalOld = 0;
int RRvalNew = 0;
int LRvalOld = 0;
int LRvalNew = 0;

// value for the ground speed sensor to increment during interrupts
int groundSpeedCount = 0;

// timer ISR
void IRAM_ATTR TimerInt()
{
	portENTER_CRITICAL_ISR(&timerMux);
	timerIntFlag = true;
	portEXIT_CRITICAL_ISR(&timerMux);
}

// helper function for encoder directions
void GetWheelDir(int* oldval, int* newVal, Movement* wheelDir, uint8_t enc1, uint8_t enc2)
{
	// store previous reding
	*oldval = *newVal;

	// get new reading and convert binary input to decimal value
	*newVal = digitalRead(enc1) * 2 + digitalRead(enc2);

	// get direction from matrix using pattern of old and new values
	Movement temp = QEM[*oldval * 4 + *newVal];

	// ignore invalid readings
	if (temp == X || temp == stopped) return;

	*wheelDir = temp;
}

// Encoder ISRs:

void IRAM_ATTR EncoderIntLF()
{
	GetWheelDir(&LFvalOld, &LFvalNew, &rpms.FL_Wheel_movement, GPIO_NUM_34, GPIO_NUM_32);
	portENTER_CRITICAL_ISR(&encoderMux);
	encIntFlag = true;
	portEXIT_CRITICAL_ISR(&encoderMux);
}

void IRAM_ATTR EncoderIntRF()
{
	GetWheelDir(&RFvalOld, &RFvalNew, &rpms.FR_Wheel_movement, GPIO_NUM_33, GPIO_NUM_35);
	portENTER_CRITICAL_ISR(&encoderMux);
	encIntFlag = true;
	portEXIT_CRITICAL_ISR(&encoderMux);
}

void IRAM_ATTR EncoderIntRR()
{
	GetWheelDir(&RRvalOld, &RRvalNew, &rpms.BR_Wheel_movement, GPIO_NUM_25, GPIO_NUM_14);
	portENTER_CRITICAL_ISR(&encoderMux);
	encIntFlag = true;
	portEXIT_CRITICAL_ISR(&encoderMux);
}

void IRAM_ATTR EncoderIntLR()
{
	GetWheelDir(&LRvalOld, &LRvalNew, &rpms.BL_Wheel_movement, GPIO_NUM_27, GPIO_NUM_26);
	portENTER_CRITICAL_ISR(&encoderMux);
	encIntFlag = true;
	portEXIT_CRITICAL_ISR(&encoderMux);
}

// ground speed sensor ISR
void IRAM_ATTR GroundSpeedISR()
{
	groundSpeedCount++;
}

void ReadSerialPayload()
{
	String payload = "";

	// read payload from slave board
	if (Serial2.available())
	{
		payload = Serial2.readStringUntil('!');

		if (payload.length() < 12) return;

		//payload.trim();

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
	}
}

// Main runs on core 1
void Main()
{
	Serial.begin(115200);
	Serial2.begin(115200);
	Serial1.begin(115200, SERIAL_8N1, 9, 17);

	// call all Initializations
	InitMotors();
	InitEncoders(EncoderIntLF, EncoderIntRF, EncoderIntRR, EncoderIntLR);
	TimerInterruptInit(TimerInt);
	// attach interrupt for gound speed sensor on pin 15
	attachInterrupt(15, GroundSpeedISR, RISING);

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

	// only send serial data every n intervals, dont want to send too fast
	int sendTimer = 0;
	const int sendInterval = 10;

	for (;;)
	{
		if (timerIntFlag)
		{
			portENTER_CRITICAL(&timerMux);
			timerIntFlag = false;
			portEXIT_CRITICAL(&timerMux);

			rpms = GetRPMS();

			rpms.GroundSpeedCount = groundSpeedCount;
			groundSpeedCount = 0;

			Drive(_intendedAngle, _intendedSpeed, rpms);

			//Serial.println("RPMS");
			//Serial.println(rpms.FL_RPM);
			//Serial.println(rpms.FR_RPM);
			//Serial.println(rpms.BL_RPM);
			//Serial.println(rpms.BR_RPM);

			// send data to slave board
			if (Serial1.availableForWrite() && sendTimer >= sendInterval)
			{
				// build the string in a format ready to be posted up to the webservice
				String data = "&FL_RPM="+ String(rpms.FL_RPM, 2);
				data += "&FR_RPM=" + String(rpms.FR_RPM, 2); 
				data += "&BL_RPM=" + String(rpms.BL_RPM, 2);
				data+= "&BR_RPM=" + String(rpms.BR_RPM, 2);

				//// need to add:
				//data += "&GSP="
				//data += "&TC=" 
				//data += "&ABS="
				//data += "&BIP=" 

				Serial1.print(data + "!");
				sendTimer = 0;
			}
			sendTimer++;
		}

		if (encIntFlag)
		{
			portENTER_CRITICAL_ISR(&encoderMux);
			encIntFlag = false;
			portEXIT_CRITICAL_ISR(&encoderMux);

		/*	Serial.printf("\nLF: %d", rpms.FL_Wheel_movement);		
			Serial.printf("\nRF: %d", rpms.FR_Wheel_movement);	
			Serial.printf("\nRR: %d", rpms.BR_Wheel_movement);		
			Serial.printf("\nLR: %d", rpms.BL_Wheel_movement);	*/	
		}
	}
}
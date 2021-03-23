// 
// File: Main.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "Main.h"
#include "mutex"

std::mutex _mutex; // used for locking values used between cores

// values for driving, from the database/serial board
int _intendedAngle = 0;
int _intendedSpeed = 0;
int _timeStamp = 1;
int _absLevel = 0;
int _tcLevel = 0;

// value for the ground speed sensor to increment during interrupts
int _groundSpeedCount = 0;

// values for determining the direction of wheel spin using the matrix above
int LFvalOld = 0;
int LFvalNew = 0;
int RFvalOld = 0;
int RFvalNew = 0;
int RRvalOld = 0;
int RRvalNew = 0;
int LRvalOld = 0;
int LRvalNew = 0;

// speed and direction of each wheel
RPMS _rpms = { 0,0,0,0,stopped,stopped,stopped,stopped };

// interrupt stuff:
volatile bool timerIntFlag = false;	// flag for use in main for actual code to run every interrupt interval

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // used for syncing main and isr, ignore this red squiggle, still works

// Quadrature Encoder Matrix courtesy of: https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf
Movement QEM[16] = { stopped,backward,forward,X,
					 forward,stopped,X,backward,
					 backward,X,stopped,forward,
					 X,forward,backward,stopped };

// timer ISR, all functionality in main
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
	GetWheelDir(&LFvalOld, &LFvalNew, &_rpms.FL_Wheel_movement, GPIO_NUM_34, GPIO_NUM_32);
}

void IRAM_ATTR EncoderIntRF()
{
	GetWheelDir(&RFvalOld, &RFvalNew, &_rpms.FR_Wheel_movement, GPIO_NUM_33, GPIO_NUM_35);
}

void IRAM_ATTR EncoderIntRR()
{
	GetWheelDir(&RRvalOld, &RRvalNew, &_rpms.BR_Wheel_movement, GPIO_NUM_25, GPIO_NUM_14);
}

void IRAM_ATTR EncoderIntLR()
{
	GetWheelDir(&LRvalOld, &LRvalNew, &_rpms.BL_Wheel_movement, GPIO_NUM_27, GPIO_NUM_26);
}

// ground speed sensor ISR
void IRAM_ATTR GroundSpeedISR()
{
	_groundSpeedCount++;
}

void ReadSerialPayload()
{
	String payload = "";

	// read payload from slave board
	if (Serial2.available())
	{
		payload = Serial2.readStringUntil('!');

		if (payload.length() < 12) return;

		//Serial.println(payload);

		JSONVar jason = JSON.parse(payload);		

		int tempStamp = atoi(jason["t"]);

		if (tempStamp > _timeStamp || (_timeStamp - tempStamp > 5000))
		{
			_mutex.lock();
			_intendedAngle = atoi(jason["a"]);
			_intendedSpeed = atoi(jason["s"]);
			_tcLevel = atoi(jason["tc"]);
			_absLevel = atoi(jason["abs"]);
			_timeStamp = tempStamp;
			_mutex.unlock();
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
	const int sendInterval = 12;
	int sendTimer = 0;

	for (;;)
	{
		if (timerIntFlag)
		{
			portENTER_CRITICAL(&timerMux);
			timerIntFlag = false;
			portEXIT_CRITICAL(&timerMux);

			GetRPMS(&_rpms);

			_rpms.GroundSpeedCount = _groundSpeedCount;

			ActiveControls actives = { false,false,0 };
			actives = Drive(_intendedAngle, _intendedSpeed, _rpms, _tcLevel, _absLevel);

			// send data to slave board every n intervals
			if (Serial1.availableForWrite() && sendTimer >= sendInterval)
			{
				// build the string in a format ready to be posted up to the webservice
				String data = "carID=1";
				data += "&FL_RPM=" + String((int)_rpms.FL_RPM);
				data += "&FR_RPM=" + String((int)_rpms.FR_RPM);
				data += "&BL_RPM=" + String((int)_rpms.BL_RPM);
				data += "&BR_RPM=" + String((int)_rpms.BR_RPM);
				data += "&GSP=" + String(_groundSpeedCount);
				data += "&TC=" + String(actives.tcActivated);
				data += "&ABS=" + String(actives.absActivated);
				data += "&BIP=" + String(actives.burnout);

				//Serial.println(data);
				Serial1.print(data + "!");
				sendTimer = 0;

				_groundSpeedCount = 0; // reset after sending val to slave board
			}
			sendTimer++;
		}
	}
}
// 
// 
// 

#include "Functions.h"

const int motorOnePin = 22;  // 22 is GPIO22
const int motorTwoPin = 23;  // 23 is GPIO23

//PWM properties
const int motorOneFreq = 10000;
const int motorTwoFreq = 10000;
const int motorOneChannel = 0;
const int motorTwoChannel = 1;
const int resolution = 8;

void setupFuncTestCpp()
{
	// configure pwm functionalities with prebuilt ledcSetup - TODO port over own in C
	ledcSetup(motorOneChannel, motorOneFreq, resolution);
	ledcSetup(motorTwoChannel, motorTwoFreq, resolution);

	// attach the channel to the GPIO
	ledcAttachPin(motorOnePin, motorOneChannel);
	ledcAttachPin(motorTwoPin, motorTwoChannel);
}

void motorLogicCpp()
{
	// ramp rpm up
	for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++) {
		// changing motor rpm with pwm
		ledcWrite(motorOneChannel, dutyCycle);
		ledcWrite(motorTwoChannel, 255 - dutyCycle);
		delay(10);
	}
	delay(500);
	// rpm down
	for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--) {
		// changing motor rpm with pwm
		ledcWrite(motorOneChannel, dutyCycle);
		ledcWrite(motorTwoChannel, 255 - dutyCycle);
		delay(10);
	}
	delay(500);
}


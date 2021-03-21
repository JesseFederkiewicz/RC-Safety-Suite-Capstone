// TorqueVectoring.h

#ifndef _TORQUEVECTORING_h
#define _TORQUEVECTORING_h

#include "Motors.h"
#include "Encoders.h"

typedef enum Accel_Or_Decel{
	accel,
	decel
};

typedef struct Current_Wheel_Direction {
	Accel_Or_Decel FrontLeft;
	Accel_Or_Decel BackLeft;
	Accel_Or_Decel FrontRight;
	Accel_Or_Decel BackRight;
};

typedef enum Wheel {
	LF,
	RF,
	LR,
	RR
};

typedef struct WheelAndRPM {
	float rpm;
	float desiredRpm;
	Wheel wheel;
};

void Drive(int angle, uint speedIn, RPMS rpm);
void Steering(int angle, uint speedRequest, RPMS rpm);
void Brake(RPMS rpm);
void SingleWheelBrake(float currentRPM, float brakeToRPM, float* wheelDuty, Movement currentMovement, Movement* lastWheelDir, int* stopCounter, int* stopDetection, Accel_Or_Decel* currentDirection);

#endif
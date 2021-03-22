// TorqueVectoring.h

#ifndef _TORQUEVECTORING_h
#define _TORQUEVECTORING_h

#include "Motors.h"
#include "Encoders.h"

//typedef enum Accel_Or_Decel{
//	accel,
//	decel
//};

//typedef struct Current_Wheel_Direction {
//	Accel_Or_Decel FrontLeft;
//	Accel_Or_Decel BackLeft;
//	Accel_Or_Decel FrontRight;
//	Accel_Or_Decel BackRight;
//};

typedef enum Wheel {
	LF,
	RF,
	LR,
	RR
};

typedef struct WheelAndRPMs {
	float rpm;
	float desiredRpm;
	Wheel wheel;
	Movement movement;
};

void Drive(int angle, uint speedIn, RPMS rpm);
void Steering(int angle, uint speedRequest, RPMS rpm);
void UpdateDuty(WheelAndRPMs currentWheel, float* duty, WheelAndRPMs slowestWheel, uint speedRequest);
void Brake(RPMS rpm);
void SingleWheelBrake(WheelAndRPMs wheel, float* wheelDuty, Movement* lastWheelDir,
						int* stopCounter, int* stopDetection);

#endif
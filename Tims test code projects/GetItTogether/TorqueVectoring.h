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

typedef struct ActivedControls {
	bool tcActivated;
	bool absActivated;
	int burnout;
};

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

ActivedControls Drive(int angle, uint speedIn, RPMS rpm, int tcLevel, int absLevel);
ActivedControls Steering(int angle, uint speedRequest, RPMS rpm, int tcLevel, int absLevel);
bool UpdateDuty(WheelAndRPMs currentWheel, float* duty, WheelAndRPMs slowestWheel, uint speedRequest, int tcLevel, int absLevel);
//void Brake(RPMS rpm);
bool SingleWheelBrake(WheelAndRPMs wheel, float* wheelDuty, Movement* lastWheelDir,
						int* stopCounter, int* stopDetection, int absLevel);

#endif
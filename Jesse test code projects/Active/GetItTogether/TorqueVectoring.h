// TorqueVectoring.h

#ifndef _TORQUEVECTORING_h
#define _TORQUEVECTORING_h

#include "Motors.h"
#include "Encoders.h"

typedef struct ActiveControls {
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

ActiveControls Drive(int angle, uint speedIn, RPMS rpm, int tcLevel, int absLevel, int brakeStrength);
ActiveControls Steering(int angle, uint speedRequest, RPMS rpm, int tcLevel, int absLevel);
bool UpdateDuty(WheelAndRPMs currentWheel, float* duty, WheelAndRPMs slowestWheel, uint speedRequest, int tcLevel, int absLevel);
bool SingleWheelBrake(WheelAndRPMs wheel, float* wheelDuty, Movement* lastWheelDir,
						int* stopCounter, int* stopDetection, int absLevel, int brakeStrength);
#endif
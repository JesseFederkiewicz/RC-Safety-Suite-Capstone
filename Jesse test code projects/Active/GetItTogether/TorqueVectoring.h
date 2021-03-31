// TorqueVectoring.h

#ifndef _TORQUEVECTORING_h
#define _TORQUEVECTORING_h

#include "Motors.h"
#include "Encoders.h"

// used to determine when abs,tc or burnouts are active, and passing this info back to the database
typedef struct ActiveControls {
	bool tcActivated;
	bool absActivated;
	int burnout;
};

// which wheel is currently being modified
typedef enum Wheel {
	LF,
	RF,
	LR,
	RR
};

// contains values for current/desired rpms, and direction for a specific wheel
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
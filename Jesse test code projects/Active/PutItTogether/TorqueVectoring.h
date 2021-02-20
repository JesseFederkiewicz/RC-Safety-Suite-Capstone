// TorqueVectoring.h

#ifndef _TORQUEVECTORING_h
#define _TORQUEVECTORING_h

#include "Motor_Encoder.h"

typedef enum Vehicle_Movement {
	forward,
	backward,
	stopped
};

void DrivingWithBrakesAndSteering(int angle, uint speedIn, RPMS rpm);

#endif
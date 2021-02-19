// TorqueVectoring.h

#ifndef _TORQUEVECTORING_h
#define _TORQUEVECTORING_h

#include "Motors.h"
#include "Encoders.h"

typedef enum Vehicle_Movement {
	forward,
	backward,
	stopped
};

void DrivingWithBrakesAndSteering(int angle, uint speedIn, RPMS rpm);

#endif
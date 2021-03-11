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

void Drive(int angle, uint speedIn, RPMS rpm);
void Steering(int angle, uint speedRequest, RPMS rpm);
void Brake(RPMS rpm);

#endif
// 
// File: TorqueVectoring.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "TorqueVectoring.h"


void SimpleSteering(int angle, uint speedIn)
{
	// since first 40% of duty cycle doesnt turn motors,
	// convert % speed to range of 40-100 for more precise control
	// out = ((55/100) * in) + 45

	float speed = speedIn == 0 ? 0 : (0.55 * speedIn) + 45.0;

	MotorDuties duties;
	MotorDirection leftDir;
	MotorDirection rightDir;

	duties.FL_Duty = speed;
	duties.FR_Duty = speed;
	duties.BL_Duty = speed;
	duties.BR_Duty = speed;

	//Go forward
	if (angle > -10 && angle < 10)
	{
		leftDir = Forward;
		rightDir = Forward;
	}

	//Burn right
	else if (angle > 80 && angle < 100)
	{
		leftDir = Forward;
		rightDir = Reverse;
	}

	//Burn left
	else if (angle < -80 && angle > -100)
	{
		leftDir = Reverse;
		rightDir = Forward;
	}

	//Go backward
	else if (angle < -110 || angle > 110)
	{
		leftDir = Reverse;
		rightDir = Reverse;
	}

	//Unknown state condition, stop it
	else
	{
		duties.FL_Duty = 0;
		duties.FR_Duty = 0;
		duties.BL_Duty = 0;
		duties.BR_Duty = 0;
	}

	SetMotorSpeeds(duties);
	SetMotorDirections(leftDir, rightDir);
}
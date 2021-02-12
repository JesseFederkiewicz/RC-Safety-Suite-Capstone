// 
// File: TorqueVectoring.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "TorqueVectoring.h"

Vehicle_Movement vehicleDir = stopped;

// this function will drive the vehicle and 
// apply brakes when zero speedSet is requested and rpms are still high
void DrivingWithBrakesAndSteering(int angle, uint speedRequest, RPMS rpm)
{
	//Serial.printf("\nspeedIn: %d", speedRequest);
	//Serial.printf("\nangleIn: %d\n", angle);

	const float dutyMin = 50.0; // the lowest duty cycle to spin the motors at, as any less wont turn them
	const float maxAng = 90.0;  // the maximum angle for each quadrant of turning

	// since first 40% of duty cycle doesnt turn motors,
	// convert % speedSet to range of 40-100 for more precise control
	// out = ((50/100) * in) + 50
	float speedSet = speedRequest == 0 ? 0 : (0.5 * speedRequest) + dutyMin;

	MotorDuties duties;
	MotorDirection leftDir;
	MotorDirection rightDir;

	duties.FL_Duty = speedSet;
	duties.FR_Duty = speedSet;
	duties.BL_Duty = speedSet;
	duties.BR_Duty = speedSet;

	// Go forward with turning
	if (angle > -90 && angle < 90 && speedSet > 0)
	{
		leftDir = Forward;
		rightDir = Forward;
		vehicleDir = forward;

		// slow down one side wheels by angle taken as %, used as a multiplier
		// duty = speedSet * ((100/90 * (90-angle))/100)

		if (angle > 0) {
			float slowDown;
			slowDown = (100.0 / maxAng * (maxAng - angle)) / 100;
			duties.BR_Duty = speedSet * slowDown;
			duties.FR_Duty = speedSet * slowDown;
		}
		else if (angle < 0)
		{
			float slowDown;
			slowDown = (100.0 / maxAng * (maxAng + angle)) / 100;
			duties.BL_Duty = speedSet * slowDown;
			duties.FL_Duty = speedSet * slowDown;
		}
	}

	// Go backward with turning
	if (angle < -90 || angle > 90 && speedSet > 0)
	{
		leftDir = Reverse;
		rightDir = Reverse;
		vehicleDir = backward;

		// slow down one side wheels by angle taken as %, used as a multiplier
		// duty = speedSet * ((100/90 * (90-angle))/100)

		if (angle < -90) {
			// convert reverse angle to forward angle(0-90) for easy math
			int angle2 = angle + 180;
			float slowDown;
			slowDown = (100.0 / maxAng * (maxAng - angle2)) / 100;
			duties.BL_Duty = speedSet * slowDown;
			duties.FL_Duty = speedSet * slowDown;
		}
		else if (angle > 90)
		{
			// convert reverse angle to forward angle(0-90) for easy math
			int angle2 = 180 - angle;
			float slowDown;
			slowDown = (100.0 / maxAng * (maxAng - angle2)) / 100;
			duties.BR_Duty = speedSet * slowDown;
			duties.FR_Duty = speedSet * slowDown;
		}
	}

	// Burn right
	else if (angle == 90)
	{
		leftDir = Forward;
		rightDir = Reverse;
		vehicleDir = stopped;
	}

	// Burn left
	else if (angle == -90)
	{
		leftDir = Reverse;
		rightDir = Forward;
		vehicleDir = stopped;
	}

	// BRAKES

	// if stop is requested and any wheel is turning
	if (speedSet == 0 && (rpm.BL_RPM > 10 || rpm.BR_RPM > 10 || rpm.FL_RPM > 10 || rpm.FR_RPM > 10))
	{
		//Serial.println("braking");		

		// brake speed from avgRPM as % of max RPM (1450)
		// range of brake speed from 50-100%
		// B = ((100-40/1450)*avgRPM) + 40

		float brakeSpeed = ((60.0 / 1450.0) * ((rpm.BL_RPM + rpm.BR_RPM + rpm.FL_RPM + rpm.FR_RPM) / 4)) + dutyMin;

		switch (vehicleDir)
		{
			// vehicle is moving forward, set motors to reverse
		case forward:
			leftDir = Reverse;
			rightDir = Reverse;
			vehicleDir = backward;
			break;

			// vehicle is moving backwards, set motors to forwards
		case backward:
			leftDir = Forward;
			rightDir = Forward;
			vehicleDir = forward;
			break;

			// vehicle is stopped, no change
		case stopped:
			brakeSpeed = 0;
			break;
		}
		duties.FL_Duty = brakeSpeed;
		duties.FR_Duty = brakeSpeed;
		duties.BL_Duty = brakeSpeed;
		duties.BR_Duty = brakeSpeed;
	}
	// if any wheel comes to a stop, assume fully stopped
	else if (speedSet == 0 && (rpm.BL_RPM == 0 || rpm.BR_RPM == 0 || rpm.FL_RPM == 0 || rpm.FR_RPM == 0))
	{
		//Serial.println("Stopped");
		vehicleDir == stopped;
	}

	SetMotorDirections(leftDir, rightDir);
	SetMotorSpeeds(duties);
}
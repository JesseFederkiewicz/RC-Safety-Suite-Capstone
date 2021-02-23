// 
// File: TorqueVectoring.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "TorqueVectoring.h"

int tempcount = 0; //take out
bool firstTime = true;

MotorDuties duties;

Vehicle_Movement vehicleDir = stopped;

// this function will drive the vehicle and 
// apply brakes when zero speedSet is requested and rpms are still high
void DrivingWithBrakesAndSteering(int angle, uint speedRequest, RPMS rpm)
{
	const float dutyMin = 50.0; // the lowest duty cycle to spin the motors at, as any less wont turn them
	const float maxAng = 90.0;  // the maximum angle for each quadrant of turning

	float speedSet = speedRequest == 0 ? 0 : (0.5 * speedRequest) + dutyMin;

	//Motorduties duties;
	MotorDirection leftDir;
	MotorDirection rightDir;

	if (firstTime)
	{
		duties.FL_Duty = 0;
		duties.FR_Duty = 0;
		duties.BL_Duty = 0;
		duties.BR_Duty = 0;
		firstTime = false;
	}


	const int MAXRPM = 1300;
	const int FL_SPEED = MAXRPM / speedRequest;


	//			0
	// -90				90
	//		   180


	//45 & speed == 100
	//Left = full
	//Right = half
	//

	// = (calculate desired rpm at 100%,  mult by speedRequest

	//calculate desired rpm at 100%

	//int duty = speedSet * ((100 / 90 * (90 - angle)) / 100);

	float desired_LEFT_RPM;
	float desired_RIGHT_RPM;

	if (angle >= 0 && angle < 90)
	{
		//Serial.println("Forward Right");
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - angle)) / 100.0);
		//				45 - speed 100
		//	1300	650	
		//					1300 * .5 * 1 = 1300 != 650

		//desired_LEFT_RPM = MAXRPM * angleAsPercent * (speedRequest / 100);

		//desired_RIGHT_RPM = MAXRPM / 2;

		leftDir = Forward;
		rightDir = Forward;

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);

		//Serial.println(desired_LEFT_RPM);
		//desired_RIGHT_RPM = MAXRPM * angleAsPercent * (double)(speedRequest / 100);

		//					1300 * .5 = 650
		desired_RIGHT_RPM = desired_LEFT_RPM * angleAsPercent;
	}

	else if (angle < 0 && angle > -90)
	{
		//Serial.println("Forward Left");
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - (angle * -1))) / 100.0);

		leftDir = Forward;
		rightDir = Forward;

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM * angleAsPercent;
	}


	//			0
	// -90				90
	//		   180


	else if (angle > 90 && angle <= 180)
	{
		//Serial.println("Backwards Right");
		double angleAsPercent = 1 - (((100.0 / 90.0 * (180.0 - angle)) / 100.0));
		// 45 - 100
		// MAX - RIGHT MAX / 2
		//
		//	
		//

		leftDir = Reverse;
		rightDir = Reverse;

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_RIGHT_RPM = desired_LEFT_RPM * angleAsPercent;
	}

	else if (angle < -90 && angle > -180)
	{
		//Serial.println("Backwards Left");
		double angleAsPercent = 1 - (((100.0 / 90.0 * (180.0 - (angle * -1))) / 100.0));

		leftDir = Reverse;
		rightDir = Reverse;

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM * angleAsPercent;
	}

	//Burn right
	if (angle == 90)
	{

		double angleAsPercent = ((100.0 / 90.0 * (90.0 - angle)) / 100.0);

		leftDir = Forward;
		rightDir = Reverse;

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);

		desired_RIGHT_RPM = desired_LEFT_RPM; //not the same rpm
	}

	//Burn left
	else if (angle == -90)
	{
		//Serial.println("Forward Left");
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - (angle * -1))) / 100.0);

		leftDir = Reverse;
		rightDir = Forward;

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM;	//not the same rpm
	}

	//duties.BL_Duty = rpm.BL_RPM < desired_LEFT_RPM ? duties.BL_Duty++ : duties.BL_Duty--;
	//duties.FL_Duty = rpm.FL_RPM < desired_LEFT_RPM ? duties.FL_Duty++ : duties.FL_Duty--;
	//duties.BR_Duty = rpm.BR_RPM < desired_RIGHT_RPM ? duties.BR_Duty++ : duties.BR_Duty--;
	//duties.FR_Duty = rpm.FR_RPM < desired_RIGHT_RPM ? duties.FR_Duty++ : duties.FR_Duty--;

	if (rpm.BL_RPM < desired_LEFT_RPM && duties.BL_Duty < 100)
	{
		if (duties.BL_Duty < dutyMin)
			duties.BL_Duty = dutyMin;

		else
			duties.BL_Duty++;
	}

	else if (rpm.BL_RPM > desired_LEFT_RPM && duties.BL_Duty > 0)
		duties.BL_Duty--;

	if (rpm.FL_RPM < desired_LEFT_RPM && duties.FL_Duty < 100)
	{
		if (duties.FL_Duty < dutyMin)
			duties.FL_Duty = dutyMin;

		else
			duties.FL_Duty++;
	}

	else if (rpm.FL_RPM > desired_LEFT_RPM && duties.FL_Duty > 0)
		duties.FL_Duty--;

	if (rpm.BR_RPM < desired_RIGHT_RPM && duties.BR_Duty < 100)
	{
		if (duties.BR_Duty < dutyMin)
			duties.BR_Duty = dutyMin;

		else
			duties.BR_Duty++;
	}

	else if (rpm.BR_RPM > desired_RIGHT_RPM && duties.BR_Duty > 0)
		duties.BR_Duty--;

	if (rpm.FR_RPM < desired_RIGHT_RPM && duties.FR_Duty < 100)
	{
		if (duties.FR_Duty < dutyMin)
			duties.FR_Duty = dutyMin;

		else
			duties.FR_Duty++;
	}

	else if (rpm.FR_RPM > desired_RIGHT_RPM && duties.FR_Duty > 0)
		duties.FR_Duty--;

	//if (tempcount == 23)

	//Serial.print("BL_RPM: ");
	//Serial.println(rpm.BL_RPM);

	//Serial.print("Des Left: ");
	//Serial.println(desired_LEFT_RPM);

	//Serial.print("Left Duty: ");
	//Serial.println(duties.BL_Duty);

	//Serial.println();


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
	//if any wheel comes to a stop, assume fully stopped
	if (speedSet == 0 && (rpm.BL_RPM < 10 || rpm.BR_RPM < 10 || rpm.FL_RPM < 10 || rpm.FR_RPM < 10))
	{
		//Serial.println("Stopped");
		vehicleDir = stopped;
	}

	//if (rpm.BL_RPM < desired_LEFT_RPM)
	//{
	//	duties.BL_Duty++;
	//}

	SetMotorDirections(leftDir, rightDir);
	SetMotorSpeeds(duties);
}
// 
// File: TorqueVectoring.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "TorqueVectoring.h"

int tempcount = 0; //take out
bool firstTime = true;
bool brakingInProgress = false;

MotorDuties duties;

Current_Wheel_Direction curWheelDirection;

//Movement vehicleDir = stopped;
//Movement lastDir = stopped;

MotorDirection leftDir;
MotorDirection rightDir;


void Steering(int angle, uint speedRequest, RPMS rpm)
{
	const float dutyMin = 50.0; // the lowest duty cycle to spin the motors at, as any less wont turn them
	const float maxAng = 90.0;  // the maximum angle for each quadrant of turning

	float speedSet = speedRequest == 0 ? 0 : (0.5 * speedRequest) + dutyMin;

	//Motorduties duties;
	//MotorDirection leftDir;
	//MotorDirection rightDir;

	if (firstTime)
	{
		duties.FL_Duty = 0;
		duties.FR_Duty = 0;
		duties.BL_Duty = 0;
		duties.BR_Duty = 0;

		curWheelDirection.BackLeft = decel;
		curWheelDirection.BackRight = decel;
		curWheelDirection.FrontLeft = decel;
		curWheelDirection.FrontRight = decel;

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


	if (!brakingInProgress && speedRequest != 0)
	{
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
			//vehicleDir = forward;
			//Serial.println("Forward");
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

			//vehicleDir = forward;
			//Serial.println("Forward");

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
			//vehicleDir = backward;

			////Serial.println("Backward");

			leftDir = Reverse;
			rightDir = Reverse;

			desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);
			desired_RIGHT_RPM = desired_LEFT_RPM * angleAsPercent;
		}

		else if (angle < -90 && angle > -180)
		{
			//Serial.println("Backwards Left");
			double angleAsPercent = 1 - (((100.0 / 90.0 * (180.0 - (angle * -1))) / 100.0));


			//vehicleDir = backward;
			////Serial.println("Backward");

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

		int breakStrengh = 5;		//higher is stronger

		float guessLeftDuty = desired_LEFT_RPM * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + dutyMin - 20;
		float guessRightDuty = desired_RIGHT_RPM * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + dutyMin - 20;


		//Serial.println(guessLeftDuty);
		//Serial.println(guessRightDuty);

		if (rpm.BL_RPM < desired_LEFT_RPM && duties.BL_Duty < 100)
		{
			if (curWheelDirection.BackLeft != accel)
			{
				if (duties.BL_Duty > guessLeftDuty)
					duties.BL_Duty++;

				else {
					duties.BL_Duty = guessLeftDuty;
					curWheelDirection.BackLeft = accel;
				}
			}

			//if (duties.BL_Duty < dutyMin)
			//	duties.BL_Duty = dutyMin;

			else
				duties.BL_Duty++;
		}

		else if (rpm.BL_RPM > desired_LEFT_RPM && duties.BL_Duty > 0)
		{
			//duties.BL_Duty--;

			//duties.BL_Duty = duties.BL_Duty - dutyMin / breakStrengh;
			duties.BL_Duty--;

			curWheelDirection.BackLeft = decel;

			//if (leftDir == Forward)
			//	leftDir = Reverse;

			//else if (leftDir == Reverse)
			//	leftDir = Forward;

			//if (rightDir == Forward)
			//	rightDir = Reverse;

			//else if (rightDir == Reverse)
			//	rightDir = Forward;

		}

		if (rpm.FL_RPM < desired_LEFT_RPM && duties.FL_Duty < 100)
		{
			//if (duties.FL_Duty < dutyMin)
			//	duties.FL_Duty = dutyMin;

			if (curWheelDirection.FrontLeft != accel)
			{
				if (duties.FL_Duty > guessLeftDuty)
					duties.FL_Duty++;
				else {
					duties.FL_Duty = guessLeftDuty;
					curWheelDirection.FrontLeft = accel;
				}
		
			}

			else
				duties.FL_Duty++;
		}

		else if (rpm.FL_RPM > desired_LEFT_RPM && duties.FL_Duty > 0)
		{
			////duties.FL_Duty--;
			//duties.FL_Duty = duties.FL_Duty - dutyMin / breakStrengh;
			duties.FL_Duty--;

			curWheelDirection.FrontLeft = decel;

			//if (leftDir == Forward)
			//	leftDir = Reverse;

			//else if (leftDir == Reverse)
			//	leftDir = Forward;

			//if (rightDir == Forward)
			//	rightDir = Reverse;

			//else if (rightDir == Reverse)
			//	rightDir = Forward;
		}

		if (rpm.BR_RPM < desired_RIGHT_RPM && duties.BR_Duty < 100)
		{
			//if (duties.BR_Duty < dutyMin)
			//	duties.BR_Duty = dutyMin;

			if (curWheelDirection.BackRight != accel)
			{
				if (duties.BR_Duty > guessRightDuty)
					duties.BR_Duty++;
				else {
					duties.BR_Duty = guessRightDuty;
					curWheelDirection.BackRight = accel;
				}		
			}

			else
				duties.BR_Duty++;
		}

		else if (rpm.BR_RPM > desired_RIGHT_RPM && duties.BR_Duty > 0)
		{
			////duties.BR_Duty--;
			//duties.BR_Duty = duties.BR_Duty - dutyMin / breakStrengh;

			duties.BR_Duty--;

			curWheelDirection.BackRight = decel;

			//if (leftDir == Forward)
			//	leftDir = Reverse;

			//else if (leftDir == Reverse)
			//	leftDir = Forward;

			//if (rightDir == Forward)
			//	rightDir = Reverse;

			//else if (rightDir == Reverse)
			//	rightDir = Forward;
		}

		if (rpm.FR_RPM < desired_RIGHT_RPM && duties.FR_Duty < 100)
		{
			//if (duties.FR_Duty < dutyMin)
			//	duties.FR_Duty = dutyMin;

			if (curWheelDirection.FrontRight != accel)
			{
				if (duties.FR_Duty > guessRightDuty)
					duties.FR_Duty++;
				else {
					duties.FR_Duty = guessRightDuty;
					curWheelDirection.FrontRight = accel;
				}
			}

			else
				duties.FR_Duty++;
		}

		else if (rpm.FR_RPM > desired_RIGHT_RPM && duties.FR_Duty > 0)
		{
			//duties.FR_Duty--;
			//duties.FR_Duty = duties.FR_Duty - dutyMin / breakStrengh;
			duties.FR_Duty--;

			curWheelDirection.FrontRight = decel;

			//if (leftDir == Forward)
			//	leftDir = Reverse;

			//else if (leftDir == Reverse)
			//	leftDir = Forward;

			//if (rightDir == Forward)
			//	rightDir = Reverse;

			//else if (rightDir == Reverse)
			//	rightDir = Forward;
		}
		//Serial.printf("\n\n\nBL Duty: %f2 - Guess: %f2 - RPM: %f2 - targetRPM: %f2\n", duties.BL_Duty, guessLeftDuty, rpm.BL_RPM, desired_LEFT_RPM);
		//Serial.printf("\nFL Duty: %f2 - Guess: %f2 - RPM: %f2 - targetRPM: %f2\n", duties.FL_Duty, guessLeftDuty, rpm.FL_RPM, desired_LEFT_RPM);
		//Serial.printf("\nBR Duty: %f2 - Guess: %f2 - RPM: %f2 - targetRPM: %f2\n", duties.BR_Duty, guessRightDuty, rpm.BR_RPM, desired_RIGHT_RPM);
		//Serial.printf("\nFR Duty: %f2 - Guess: %f2 - RPM: %f2 - targetRPM: %f2\n", duties.FR_Duty, guessRightDuty, rpm.FR_RPM, desired_RIGHT_RPM);
	}

	//if (tempcount == 23)

	//Serial.print("BL_RPM: ");
	//Serial.println(rpm.BL_RPM);

	//Serial.print("Des Left: ");
	//Serial.println(desired_LEFT_RPM);

	//Serial.print("Left Duty: ");
	//Serial.println(duties.BL_Duty);

	//Serial.println();

	if (speedRequest == 0) {
		duties.BL_Duty = 0;
		duties.BR_Duty = 0;
		duties.FL_Duty = 0;
		duties.FR_Duty = 0;
	}

	SetMotorDirections(leftDir, rightDir);
	SetMotorSpeeds(duties);
}

void Brake(RPMS rpm)
{
	if ( (rpm.BL_RPM < 10 || rpm.BR_RPM < 10 || rpm.FL_RPM < 10 || rpm.FR_RPM < 10))
	{
		//Serial.println("Stopped");
		//lastDir = vehicleDir;
		//vehicleDir = stopped;
		brakingInProgress = false;
		//Serial.println("Stop triggered");

		duties.FL_Duty = 0;
		duties.FR_Duty = 0;
		duties.BL_Duty = 0;
		duties.BR_Duty = 0;
	}

	if ((rpm.BL_RPM > 20 || rpm.BR_RPM > 20 || rpm.FL_RPM > 20 || rpm.FR_RPM > 20))
	{

	}

	if ((rpm.BL_RPM > 10 || rpm.BR_RPM > 10 || rpm.FL_RPM > 10 || rpm.FR_RPM > 10) || brakingInProgress)//(rpm.BL_RPM > 5 || rpm.BR_RPM > 5 || rpm.FL_RPM > 5 || rpm.FR_RPM > 5))
	{
		//VehicleDir --  forward 0, backward 1
		//Serial.println(vehicleDir);

		//wheelDir
		if (!brakingInProgress)
		{
			leftDir = Reverse;
			rightDir = Reverse;
			//vehicleDir = backward;
			brakingInProgress = true;
			//brakingInProgress = true;
		}

		else if (!brakingInProgress)
		{
			leftDir = Forward;
			rightDir = Forward;
			//vehicleDir = forward;
			brakingInProgress = true;

		}

		curWheelDirection.BackLeft == decel;
		curWheelDirection.BackRight = decel;
		curWheelDirection.FrontLeft = decel;
		curWheelDirection.FrontRight = decel;

		//if (rightDir == Forward && !brakingInProgress)
		//{
		//	rightDir = Reverse;
		//	breakingInProgressRight = true;
		//	//brakingInProgress = true;
		//}

		//else if (rightDir == Reverse && !breakingInProgressRight)
		//{
		//	rightDir = Forward;
		//	breakingInProgressRight = true;
		//}


		int stopPower = 100;
		duties.FL_Duty = stopPower;
		duties.FR_Duty = stopPower;
		duties.BL_Duty = stopPower;
		duties.BR_Duty = stopPower;



		//// if stop is requested and any wheel is turning
		//if (speedSet == 0 && (rpm.BL_RPM > 5 || rpm.BR_RPM > 5 || rpm.FL_RPM > 5 || rpm.FR_RPM > 5))
		//{
		//	//Serial.println("braking");		

		//	// brake speed from avgRPM as % of max RPM (1450)
		//	// range of brake speed from 50-100%
		//	// B = ((100-40/1450)*avgRPM) + 40

		//	//float brakeSpeed = ((60.0 / 1450.0) * ((rpm.BL_RPM + rpm.BR_RPM + rpm.FL_RPM + rpm.FR_RPM) / 4)) + dutyMin;
		//	float brakeSpeed = 100;

		//	switch (vehicleDir)
		//	{
		//		// vehicle is moving forward, set motors to reverse
		//	case forward:
		//		leftDir = Reverse;
		//		rightDir = Reverse;
		//		vehicleDir = backward;
		//		break;

		//		// vehicle is moving backwards, set motors to forwards
		//	case backward:
		//		leftDir = Forward;
		//		rightDir = Forward;
		//		vehicleDir = forward;
		//		break;

		//		// vehicle is stopped, no change
		//	case stopped:
		//		brakeSpeed = 0;
		//		break;
		//	}

		//	duties.FL_Duty = brakeSpeed;
		//	duties.FR_Duty = brakeSpeed;
		//	duties.BL_Duty = brakeSpeed;
		//	duties.BR_Duty = brakeSpeed;

		//	Serial.println("Breaking");
		//}



		//else if (speedSet == 0 && vehicleDir != stopped && (rpm.BL_RPM > 10 || rpm.BR_RPM > 10 || rpm.FL_RPM > 10 || rpm.FR_RPM > 10))//(rpm.BL_RPM > 5 || rpm.BR_RPM > 5 || rpm.FL_RPM > 5 || rpm.FR_RPM > 5))
		//{
		//	//VehicleDir --  forward 0, backward 1
		//	//Serial.println(vehicleDir);

		//	//wheelDir
		//	if (vehicleDir == forward && !brakingInProgress)
		//	{
		//		leftDir = Reverse;
		//		rightDir = Reverse;
		//		vehicleDir = backward;
		//		brakingInProgress = true;
		//		//brakingInProgress = true;
		//		//Serial.println("SHORT DSAHJIODSAHIOUDSAHIOUDSA");
		//	}

		//	else if (vehicleDir == backward && !brakingInProgress)
		//	{
		//		leftDir = Forward;
		//		rightDir = Forward;
		//		vehicleDir = forward;
		//		brakingInProgress = true;

		//		//Serial.println("DASKJIODSAJIOJDSAIJDSAIODJSAIOJdsAIOJDASIOJDSAIOJDSAIOJDSAIOJDSAIOJDSAIOJDSIAOdsa");
		//	}

		//	else
		//	{
		//		//Serial.println("WHAT WHATRNIASDHIUODSAHJUIODSA");
		//	}

		//	//if (rightDir == Forward && !brakingInProgress)
		//	//{
		//	//	rightDir = Reverse;
		//	//	breakingInProgressRight = true;
		//	//	//brakingInProgress = true;
		//	//}

		//	//else if (rightDir == Reverse && !breakingInProgressRight)
		//	//{
		//	//	rightDir = Forward;
		//	//	breakingInProgressRight = true;
		//	//}


		//	int stopPower = 60;
		//	duties.FL_Duty = stopPower;
		//	duties.FR_Duty = stopPower;
		//	duties.BL_Duty = stopPower;
		//	duties.BR_Duty = stopPower;
		//}


		//if (rpm.BL_RPM < desired_LEFT_RPM)
		//{
		//	duties.BL_Duty++;
		//}

		//rev 1 forward 0
		//Serial.println(leftDir);
		//
		////Print duties

		//Serial.println();
		//Serial.print("FR Duty: ");
		//Serial.println(duties.FR_Duty);
		//Serial.print("BR Duty: ");
		//Serial.println(duties.BR_Duty);
		//Serial.print("FL Duty: ");
		//Serial.println(duties.FL_Duty);
		//Serial.print("BL Duty: ");
		//Serial.println(duties.BL_Duty);
		//Serial.println();

	}
}

// this function will drive the vehicle and 
// apply brakes when zero speedSet is requested and rpms are still high
void Drive(int angle, uint speedRequest, RPMS rpm)
{
	Steering(angle, speedRequest, rpm);

	/*if (speedRequest == 0)
		Brake(rpm);*/
}
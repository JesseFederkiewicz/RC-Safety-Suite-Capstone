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

// this function will drive the vehicle and 
// apply brakes when zero speedSet is requested and rpms are still high
void Drive(int angle, uint speedRequest, RPMS rpm)
{
	Steering(angle, speedRequest, rpm);

	if (speedRequest == 0)
		Brake(rpm);

	SetMotorDirections(leftDir, rightDir);
	SetMotorSpeeds(duties);
}

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

	const int MAXRPM = 1800;
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

		float guessLeftDuty = desired_LEFT_RPM * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + dutyMin - 10;
		float guessRightDuty = desired_RIGHT_RPM * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + dutyMin - 10;


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
}

Movement BR_lastDir = stopped;
Movement BL_lastDir = stopped;
Movement FR_lastDir = stopped;
Movement FL_lastDir = stopped;
int BR_stopDetection = 0;
int BL_stopDetection = 0;
int FR_stopDetection = 0;
int FL_stopDetection = 0;
int BR_stopCounter = 0;
int BL_stopCounter = 0;
int FR_stopCounter = 0;
int FL_stopCounter = 0;



void Brake(RPMS rpm)
{
	//Serial.println(rpm.BL_RPM);
	//Serial.println(rpm.FL_RPM);
	//Serial.println(rpm.BR_RPM);
	//Serial.println(rpm.FL_RPM);

	//VehicleDir --  forward 0, backward 1
	//Serial.println(vehicleDir);

	Serial.println("move");
	Serial.println(rpm.BL_Wheel_movement);
	Serial.println(rpm.FL_Wheel_movement);
	Serial.println(rpm.BR_Wheel_movement);
	Serial.println(rpm.FL_Wheel_movement);

	/*		 brake speed from RPM as % of max RPM (1450)
		range of brake speed from 40-100%*/
	const float offset = 60.0;
	const int minrpm = 15;

	if (rpm.FL_RPM > minrpm)
		duties.FL_Duty = 100;
	else
		duties.FL_Duty = 0;

	if (rpm.FR_RPM > minrpm)
		duties.FR_Duty = 100;
	else
		duties.FR_Duty = 0;

	if (rpm.BL_RPM > minrpm)
		duties.BL_Duty = 100;
	else
		duties.BL_Duty = 0;

	if (rpm.BR_RPM > minrpm)
		duties.BR_Duty = 100;
	else
		duties.BR_Duty = 0;

	//if (rpm.FL_RPM > minrpm)
	//	duties.FL_Duty = (((100.0 - offset) / 1450.0) * rpm.FL_RPM) + offset;
	//else
	//	duties.FL_Duty = 0;

	//if (rpm.FR_RPM > minrpm)
	//	duties.FR_Duty = (((100.0 - offset) / 1450.0) * rpm.FL_RPM) + offset;
	//else
	//	duties.FR_Duty = 0;

	//if (rpm.BL_RPM > minrpm)
	//	duties.BL_Duty = (((100.0 - offset) / 1450.0) * rpm.FL_RPM) + offset;
	//else
	//	duties.BL_Duty = 0;

	//if (rpm.BR_RPM > minrpm)
	//	duties.BR_Duty = (((100.0 - offset) / 1450.0) * rpm.FL_RPM) + offset;
	//else
	//	duties.BR_Duty = 0;

	//Serial.println(duties.FL_Duty);
	//Serial.println(duties.FR_Duty);
	//Serial.println(duties.BL_Duty);
	//Serial.println(duties.BR_Duty);

	// reverse directions to stop wheels
	if (rpm.BL_Wheel_movement == forward) {
		leftDir = Reverse;
	}
	else if (rpm.BL_Wheel_movement == backward) {
		leftDir = Forward;
	}

	if (rpm.FL_Wheel_movement == forward) {
		leftDir = Reverse;
	}
	else if (rpm.FL_Wheel_movement == backward) {
		leftDir = Forward;
	}

	if (rpm.FR_Wheel_movement == forward) {
		rightDir = Reverse;
	}
	else if (rpm.FR_Wheel_movement == backward) {
		rightDir = Forward;
	}

	if (rpm.BR_Wheel_movement == forward) {
		rightDir = Reverse;
	}
	else if (rpm.BR_Wheel_movement == backward) {
		rightDir = Forward;
	}

	// count # of direction reversals
	if (BR_lastDir != rpm.BR_Wheel_movement)
		BR_stopDetection++;

	if (BL_lastDir != rpm.BL_Wheel_movement)
		BL_stopDetection++;

	if (FR_lastDir != rpm.FR_Wheel_movement)
		FR_stopDetection++;

	if (FL_lastDir != rpm.FL_Wheel_movement)
		FL_stopDetection++;

	// store last wheel movements
	BR_lastDir = rpm.BR_Wheel_movement;
	BL_lastDir = rpm.BL_Wheel_movement;
	FR_lastDir = rpm.FR_Wheel_movement;
	FL_lastDir = rpm.FL_Wheel_movement;

	int jiggleLimit = 3;

	Serial.println("detect");
	Serial.println(BL_stopDetection);
	Serial.println(FL_stopDetection);
	Serial.println(BR_stopDetection);
	Serial.println(FR_stopDetection);

	// if wheels reverse n times, set duties to 0
	if (BR_stopDetection >= jiggleLimit)
		duties.BR_Duty = 0;
	if (BL_stopDetection >= jiggleLimit)
		duties.BL_Duty = 0;
	if (FR_stopDetection >= jiggleLimit)
		duties.FR_Duty = 0;
	if (FL_stopDetection >= jiggleLimit)
		duties.FL_Duty = 0;

	// if rpms are 0, count it
	if (rpm.BR_Wheel_movement == stopped)
		BR_stopCounter++;
	if (rpm.BL_Wheel_movement == stopped)
		BL_stopCounter++;
	if (rpm.FR_Wheel_movement == stopped)
		FR_stopCounter++;
	if (rpm.FL_Wheel_movement == stopped)
		FL_stopCounter++;

	int stopLimit = 5;
	int rpmReset = 150;

	// if rpms are 0 n times, reset counters
	if (BR_stopCounter >= stopLimit || rpm.BR_RPM > rpmReset){
		BR_stopDetection = 0;
		BR_stopCounter = 0;
	}
	if (BL_stopCounter >= stopLimit || rpm.BL_RPM > rpmReset){
		BL_stopDetection = 0;
		BL_stopCounter = 0;
	}
	if (FR_stopCounter >= stopLimit || rpm.FR_RPM > rpmReset){
		FR_stopDetection = 0;
		FR_stopCounter = 0;
	}
	if (FL_stopCounter >= stopLimit || rpm.FL_RPM > rpmReset){
		FL_stopDetection = 0;
		FL_stopCounter = 0;
	}

	Serial.println("count");
	Serial.println(BR_stopCounter);
	Serial.println(BL_stopCounter);
	Serial.println(FR_stopCounter);
	Serial.println(FL_stopCounter);

	curWheelDirection.BackLeft = decel;
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
}


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




//
//// this function will drive the vehicle and 
//// apply brakes when zero speedSet is requested and rpms are still high
//void DrivingWithBrakesAndSteering(int angle, uint speedRequest, RPMS rpm)
//{
//	//Serial.printf("\nspeedIn: %d", speedRequest);
//	//Serial.printf("\nangleIn: %d\n", angle);
//
//	const float dutyMin = 50.0; // the lowest duty cycle to spin the motors at, as any less wont turn them
//	const float maxAng = 90.0;  // the maximum angle for each quadrant of turning
//
//	// since first 40% of duty cycle doesnt turn motors,
//	// convert % speedSet to range of 40-100 for more precise control
//	// out = ((50/100) * in) + 50
//	float speedSet = speedRequest == 0 ? 0 : (0.5 * speedRequest) + dutyMin;
//
//	MotorDuties duties;
//	MotorDirection leftDir;
//	MotorDirection rightDir;
//
//	duties.FL_Duty = speedSet;
//	duties.FR_Duty = speedSet;
//	duties.BL_Duty = speedSet;
//	duties.BR_Duty = speedSet;
//
//	// Go forward with turning
//	if (angle > -90 && angle < 90 && speedSet > 0)
//	{
//		leftDir = Forward;
//		rightDir = Forward;
//		vehicleDir = forward;
//
//		// slow down one side wheels by angle taken as %, used as a multiplier
//		// duty = speedSet * ((100/90 * (90-angle))/100)
//
//		if (angle > 0) {
//			float slowDown;
//			slowDown = (100.0 / maxAng * (maxAng - angle)) / 100;
//			duties.BR_Duty = speedSet * slowDown;
//			duties.FR_Duty = speedSet * slowDown;
//		}
//		else if (angle < 0)
//		{
//			float slowDown;
//			slowDown = (100.0 / maxAng * (maxAng + angle)) / 100;
//			duties.BL_Duty = speedSet * slowDown;
//			duties.FL_Duty = speedSet * slowDown;
//		}
//	}
//
//	// Go backward with turning
//	if (angle < -90 || angle > 90 && speedSet > 0)
//	{
//		leftDir = Reverse;
//		rightDir = Reverse;
//		vehicleDir = backward;
//
//		// slow down one side wheels by angle taken as %, used as a multiplier
//		// duty = speedSet * ((100/90 * (90-angle))/100)
//
//		if (angle < -90) {
//			// convert reverse angle to forward angle(0-90) for easy math
//			int angle2 = angle + 180;
//			float slowDown;
//			slowDown = (100.0 / maxAng * (maxAng - angle2)) / 100;
//			duties.BL_Duty = speedSet * slowDown;
//			duties.FL_Duty = speedSet * slowDown;
//		}
//		else if (angle > 90)
//		{
//			// convert reverse angle to forward angle(0-90) for easy math
//			int angle2 = 180 - angle;
//			float slowDown;
//			slowDown = (100.0 / maxAng * (maxAng - angle2)) / 100;
//			duties.BR_Duty = speedSet * slowDown;
//			duties.FR_Duty = speedSet * slowDown;
//		}
//	}
//
//	// Burn right
//	else if (angle == 90)
//	{
//		leftDir = Forward;
//		rightDir = Reverse;
//		vehicleDir = stopped;
//	}
//
//	// Burn left
//	else if (angle == -90)
//	{
//		leftDir = Reverse;
//		rightDir = Forward;
//		vehicleDir = stopped;
//	}
//
//	// BRAKES
//
//	// if stop is requested and any wheel is turning
//	if (speedSet == 0 && (rpm.BL_RPM > 10 || rpm.BR_RPM > 10 || rpm.FL_RPM > 10 || rpm.FR_RPM > 10))
//	{
//		//Serial.println("braking");		
//
//		// brake speed from avgRPM as % of max RPM (1450)
//		// range of brake speed from 50-100%
//		// B = ((100-40/1450)*avgRPM) + 40
//
//		float brakeSpeed = ((60.0 / 1450.0) * ((rpm.BL_RPM + rpm.BR_RPM + rpm.FL_RPM + rpm.FR_RPM) / 4)) + dutyMin;
//
//		switch (vehicleDir)
//		{
//			// vehicle is moving forward, set motors to reverse
//		case forward:
//			leftDir = Reverse;
//			rightDir = Reverse;
//			vehicleDir = backward;
//			break;
//
//			// vehicle is moving backwards, set motors to forwards
//		case backward:
//			leftDir = Forward;
//			rightDir = Forward;
//			vehicleDir = forward;
//			break;
//
//			// vehicle is stopped, no change
//		case stopped:
//			brakeSpeed = 0;
//			break;
//		}
//		duties.FL_Duty = brakeSpeed;
//		duties.FR_Duty = brakeSpeed;
//		duties.BL_Duty = brakeSpeed;
//		duties.BR_Duty = brakeSpeed;
//	}
//	// if any wheel comes to a stop, assume fully stopped
//	else if (speedSet == 0 && (rpm.BL_RPM == 0 || rpm.BR_RPM == 0 || rpm.FL_RPM == 0 || rpm.FR_RPM == 0))
//	{
//		//Serial.println("Stopped");
//		vehicleDir == stopped;
//	}
//
//	SetMotorDirections(leftDir, rightDir);
//	SetMotorSpeeds(duties);
//}
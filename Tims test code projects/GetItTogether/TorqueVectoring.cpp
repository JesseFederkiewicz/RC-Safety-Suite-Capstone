// 
// File: TorqueVectoring.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "TorqueVectoring.h"

int tempcount = 0; //take out
bool firstTime = true;
bool brakingInProgress = false;
bool _TC_Active = true;
bool _ABS_Active = false;

int BR_stopDetection = 0;
int BL_stopDetection = 0;
int FR_stopDetection = 0;
int FL_stopDetection = 0;

const float DUTYMIN = 50.0; // the lowest duty cycle to spin the motors at, as any less wont turn them
const int MAXRPM = 1300;

MotorDuties duties;
Current_Wheel_Direction curWheelDirection;
MotorDirection leftDir;
MotorDirection rightDir;

void UpdateDuty(WheelAndRPM currentWheel, float* duty, Accel_Or_Decel* wheelDir, WheelAndRPM slowestWheel, uint speedRequest)
{
	//Serial.printf("\n\n currentWheel: %d", currentWheel.wheel);

	//Serial.printf("\n currentRPM: %f", currentWheel.rpm);
	//Serial.printf("\n currentDesired: %f", currentWheel.desiredRpm);
	//Serial.printf("\n slowestRPM: %f", slowestWheel.rpm);
	//Serial.printf("\n slowestDesired: %f", slowestWheel.desiredRpm);

	float newDesired = currentWheel.desiredRpm;
	Serial.printf("\n\n %d", currentWheel.wheel);

	// driving with traction control
	if (_TC_Active && (currentWheel.wheel != slowestWheel.wheel))
	{
		if (currentWheel.rpm > slowestWheel.rpm + 50);
		{
			float ratio = currentWheel.desiredRpm / slowestWheel.desiredRpm;

			newDesired = slowestWheel.rpm * ratio;
		}
	}

	Serial.printf("\n Desired1: %f", newDesired);
	if (currentWheel.rpm > newDesired && *duty > 0)
	{
		Serial.printf("\n rpm: %f", currentWheel.rpm);
		Serial.printf("\n Desired3: %f", newDesired);
		*duty -= 15;

		*wheelDir = decel;
	}	
	else if (currentWheel.rpm <= newDesired && *duty < 100)
	{
		Serial.printf("\n rpm: %f", currentWheel.rpm);
		Serial.printf("\n Desired2: %f", newDesired);

		if (*duty < DUTYMIN)
			*duty = DUTYMIN;

		//if (*wheelDir != accel)
		{
			float guessDuty = newDesired * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + DUTYMIN - 30;

			if (*duty >= guessDuty)
			{
				Serial.printf("\n GUESS1: %f", guessDuty);
				*duty += 1;
			}
			else {
				Serial.printf("\n GUESS2: %f", guessDuty);
				*duty = guessDuty;
				*wheelDir = accel;
			}
		}
		/*else
			*duty += 1;*/
	}
}

// this function will drive the vehicle and 
// apply brakes when zero speedSet is requested and rpms are still high
void Drive(int angle, uint speedRequest, RPMS rpm)
{
	Steering(angle, speedRequest, rpm);

	if (speedRequest == 0) {
		Brake(rpm);
	}
	else
	{
		BR_stopDetection = 0;
		BL_stopDetection = 0;
		FR_stopDetection = 0;
		FL_stopDetection = 0;
	}

	SetMotorDirections(leftDir, rightDir);
	//Serial.printf("\n\n duties: \n %f\n %f\n %f\n %f", duties.BL_Duty, duties.BR_Duty, duties.FL_Duty, duties.FR_Duty);
	SetMotorSpeeds(duties);
}

void Steering(int angle, uint speedRequest, RPMS rpm)
{
	const float maxAng = 90.0;  // the maximum angle for each quadrant of turning

	//float speedSet = speedRequest == 0 ? 0 : (0.5 * speedRequest) + DUTYMIN;

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

		//float guessLeftDuty = desired_LEFT_RPM * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + DUTYMIN - 30;
		//float guessRightDuty = desired_RIGHT_RPM * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + DUTYMIN - 30;

		WheelAndRPM slowestWheel;
		slowestWheel.rpm = rpm.BL_RPM;
		slowestWheel.desiredRpm = desired_LEFT_RPM;
		slowestWheel.wheel = LR;

		if (rpm.BR_RPM < slowestWheel.rpm) {
			slowestWheel.rpm = rpm.BR_RPM;
			slowestWheel.desiredRpm = desired_RIGHT_RPM;
			slowestWheel.wheel = RR;
		}
		if (rpm.FL_RPM < slowestWheel.rpm) {
			slowestWheel.rpm = rpm.FL_RPM;
			slowestWheel.desiredRpm = desired_LEFT_RPM;
			slowestWheel.wheel = LF;
		}
		if (rpm.FR_RPM < slowestWheel.rpm) {
			slowestWheel.rpm = rpm.FR_RPM;
			slowestWheel.desiredRpm = desired_RIGHT_RPM;
			slowestWheel.wheel = RF;
		}


		//Serial.println(guessLeftDuty);
		//Serial.println(guessRightDuty);
		WheelAndRPM backLeft = { rpm.BL_RPM, desired_LEFT_RPM, LR };
		WheelAndRPM frontLeft = { rpm.FL_RPM, desired_LEFT_RPM, LF };
		WheelAndRPM backRight = { rpm.BR_RPM, desired_RIGHT_RPM, RR };
		WheelAndRPM frontRight = { rpm.FR_RPM, desired_RIGHT_RPM, RF };

		UpdateDuty(backLeft, &duties.BL_Duty, &curWheelDirection.BackLeft, slowestWheel,speedRequest);
		UpdateDuty(frontLeft, &duties.FL_Duty, &curWheelDirection.FrontLeft, slowestWheel, speedRequest);
		UpdateDuty(backRight, &duties.BR_Duty, &curWheelDirection.BackRight, slowestWheel, speedRequest);
		UpdateDuty(frontRight, &duties.FR_Duty, &curWheelDirection.FrontRight, slowestWheel, speedRequest);
	}
}

Movement BR_lastDir = stopped;
Movement BL_lastDir = stopped;
Movement FR_lastDir = stopped;
Movement FL_lastDir = stopped;
//int BR_stopDetection = 0;
//int BL_stopDetection = 0;
//int FR_stopDetection = 0;
//int FL_stopDetection = 0;
int BR_stopCounter = 0;
int BL_stopCounter = 0;
int FR_stopCounter = 0;
int FL_stopCounter = 0;



void Brake(RPMS rpm)
{
	//VehicleDir --  forward 0, backward 1
	//Serial.println(vehicleDir);

	//Serial.println("Breaking");
	//Serial.println(rpm.BL_RPM);
	//Serial.println(rpm.FL_RPM);
	//Serial.println(rpm.BR_RPM);
	//Serial.println(rpm.FL_RPM);

	//Serial.println(rpm.BL_Wheel_movement);
	//Serial.println(rpm.FL_Wheel_movement);
	//Serial.println(rpm.BR_Wheel_movement);
	//Serial.println(rpm.FL_Wheel_movement);

	/*		 brake speed from RPM as % of max RPM (1450)
		range of brake speed from 40-100%*/
	const float offset = 60.0;
	const int minrpm = 2;

	if (rpm.FL_RPM > minrpm)		//TODO change to by dynamic
		duties.FL_Duty = 100;
	//else
	//	duties.FL_Duty = 0;

	if (rpm.FR_RPM > minrpm)
		duties.FR_Duty = 100;
	//else
	//	duties.FR_Duty = 0;

	if (rpm.BL_RPM > minrpm)
		duties.BL_Duty = 100;
	//else
	//	duties.BL_Duty = 0;

	if (rpm.BR_RPM > minrpm)
		duties.BR_Duty = 100;
	//else
	//	duties.BR_Duty = 0;

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

	int jiggleLimit = 2;

	//Serial.println("detect");
	//Serial.println(BL_stopDetection);
	//Serial.println(FL_stopDetection);
	//Serial.println(BR_stopDetection);
	//Serial.println(FR_stopDetection);

	// if wheels reverse n times, set duties to 0
	if (BR_stopDetection >= jiggleLimit)
		duties.BR_Duty = 0;
	if (BL_stopDetection >= jiggleLimit)
		duties.BL_Duty = 0;
	if (FR_stopDetection >= jiggleLimit)
		duties.FR_Duty = 0;
	if (FL_stopDetection >= jiggleLimit)
		duties.FL_Duty = 0;

	Serial.println("Jiggle counters");
	Serial.println(BR_stopDetection);
	Serial.println(BL_stopDetection);
	Serial.println(FR_stopDetection);
	Serial.println(FL_stopDetection);

	// if rpms are 0, count it
	if (rpm.BR_Wheel_movement == stopped)		//Todo these arent adequately triggering
		BR_stopCounter++;
	if (rpm.BL_Wheel_movement == stopped)
		BL_stopCounter++;
	if (rpm.FR_Wheel_movement == stopped)
		FR_stopCounter++;
	if (rpm.FL_Wheel_movement == stopped)
		FL_stopCounter++;

	//Serial.println("Stop counters");
	//Serial.println(BL_stopCounter);
	//Serial.println(FL_stopCounter);
	//Serial.println(BR_stopCounter);
	//Serial.println(FR_stopCounter);

	int stopLimit = 2; // 2
	int rpmReset = 70; //70 OK? //20 //170 //lower number causes jittering, higher number causes no brake past low rpms

	//focus area ^ 

	// if rpms are 0 n times, reset counters
	if (BR_stopCounter >= stopLimit || rpm.BR_RPM > rpmReset) {
		BR_stopDetection = 0;
		BR_stopCounter = 0;
	}
	if (BL_stopCounter >= stopLimit || rpm.BL_RPM > rpmReset) {
		BL_stopDetection = 0;
		BL_stopCounter = 0;
	}
	if (FR_stopCounter >= stopLimit || rpm.FR_RPM > rpmReset) {
		FR_stopDetection = 0;
		FR_stopCounter = 0;
	}
	if (FL_stopCounter >= stopLimit || rpm.FL_RPM > rpmReset) {
		FL_stopDetection = 0;
		FL_stopCounter = 0;
	}

	//Higher is more brake strength
	int spinCutterStrength = 4; //4
	int spinRpm = 5;

	//Optimize reverse spin
	if (rpm.BR_RPM > spinRpm)
	{
		duties.BR_Duty -= duties.BR_Duty / spinCutterStrength;

		if (duties.BR_Duty < 40)
			duties.BR_Duty = 0;
	}

	if (rpm.BL_RPM > spinRpm)
	{
		duties.BL_Duty -= duties.BL_Duty / spinCutterStrength;

		if (duties.BL_Duty < 40)
			duties.FL_Duty = 0;
	}

	if (rpm.FR_RPM > spinRpm)
	{
		duties.FR_Duty -= duties.FR_Duty / spinCutterStrength;

		if (duties.FL_Duty < 40)
			duties.FL_Duty = 0;
	}

	if (rpm.FL_RPM > spinRpm)
	{
		duties.FL_Duty -= duties.FL_Duty / spinCutterStrength;

		if (duties.FL_Duty < 40)
			duties.FL_Duty = 0;
	}

	curWheelDirection.BackLeft = decel;
	curWheelDirection.BackRight = decel;
	curWheelDirection.FrontLeft = decel;
	curWheelDirection.FrontRight = decel;
}
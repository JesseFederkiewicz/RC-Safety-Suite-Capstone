// 
// File: TorqueVectoring.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "TorqueVectoring.h"

bool firstTime = true;    // for initializing values on the first pass
bool _TC_Active = true;   // for activating traction control
bool _ABS_Active = false; // for activating anti-lock brakes

int BR_stopDetection = 0;
int BL_stopDetection = 0;
int FR_stopDetection = 0;
int FL_stopDetection = 0;

int BR_stopCounter = 0;
int BL_stopCounter = 0;
int FR_stopCounter = 0;
int FL_stopCounter = 0;

// the last direction of wheel rotation as determined by the encoder readings/matrix calc
Movement BR_lastDir = stopped;
Movement BL_lastDir = stopped;
Movement FR_lastDir = stopped;
Movement FL_lastDir = stopped;

const float DUTYMIN = 50.0; // the lowest duty cycle to spin the motors at, as any less wont turn them
const int MAXRPM = 1300;    // wheel speed limit

Current_Wheel_Direction curWheelDirection; // each wheel is either accelerating or decelerating
MotorDuties duties;      // duty cycle values to set the motors to
MotorDirection leftDir;  // direction the left motors will be set to spin
MotorDirection rightDir; // direction of the right motors

void UpdateDuty(WheelAndRPM currentWheel, float* duty, Accel_Or_Decel* wheelDir, WheelAndRPM slowestWheel, uint speedRequest)
{
	// if traction control is enabled, update desired RPM to match slowest wheel, keeping the ratio for turns
	if (_TC_Active && (currentWheel.wheel != slowestWheel.wheel))
	{
		if (currentWheel.rpm > slowestWheel.rpm + 50);
		{
			float ratio = currentWheel.desiredRpm / slowestWheel.desiredRpm;

			currentWheel.desiredRpm = slowestWheel.rpm * ratio;
		}
	}

	// wheel too fast, decrement duty
	if (currentWheel.rpm > currentWheel.desiredRpm && *duty > 0)
	{
		*duty -= 15;
		*wheelDir = decel;

		// possibly insert brakes here
	}	

	// wheel too slow, increment duty
	else if (currentWheel.rpm <= currentWheel.desiredRpm && *duty < 100)
	{
		if (*duty < DUTYMIN)
			*duty = DUTYMIN;

		// sometimes incrementing duty by 1 is too slow, so we'll need to jump up to a higher duty right away
		float guessDuty = currentWheel.desiredRpm * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + DUTYMIN - 30;

		if (*duty >= guessDuty)
		{
			*duty += 1;
		}
		else {
			*duty = guessDuty;
			*wheelDir = accel;
		}
	}
}

// this function will apply brakes when speed requested is zero, or
// call the steering function to determine duty cycles, 
// and then set motor directions and duties
void Drive(int angle, uint speedRequest, RPMS rpm)
{
	if (speedRequest == 0) {
		//Brake(rpm);
		SingleWheelBrake(rpm.BL_RPM, 0, &duties.BL_Duty, rpm.BL_Wheel_movement, &BL_lastDir, &BL_stopCounter, &BL_stopDetection, &curWheelDirection.BackLeft);
		SingleWheelBrake(rpm.BR_RPM, 0, &duties.BR_Duty, rpm.BR_Wheel_movement, &BR_lastDir, &BR_stopCounter, &BR_stopDetection, &curWheelDirection.BackRight);
		SingleWheelBrake(rpm.FL_RPM, 0, &duties.FL_Duty, rpm.FL_Wheel_movement, &FL_lastDir, &FL_stopCounter, &FL_stopDetection, &curWheelDirection.FrontLeft);
		SingleWheelBrake(rpm.FR_RPM, 0, &duties.FR_Duty, rpm.FR_Wheel_movement, &FR_lastDir, &FR_stopCounter, &FR_stopDetection, &curWheelDirection.FrontRight);
	}
	else
	{
		Steering(angle, speedRequest, rpm);
		BR_stopDetection = 0;
		BL_stopDetection = 0;
		FR_stopDetection = 0;
		FL_stopDetection = 0;
	}

	SetMotorDirections(leftDir, rightDir);
	SetMotorSpeeds(duties);
}

// calculates desired rpms for each wheel, based on requested speed and angle, using 
void Steering(int angle, uint speedRequest, RPMS rpm)
{
	// initialize values on first pass
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
	//		  0
	// -90			90
	//		 180

	//45 & speed == 100
	//Left = full
	//Right = half
	//

	// = (calculate desired rpm at 100%,  mult by speedRequest

	//calculate desired rpm at 100%

	//int duty = speedSet * ((100 / 90 * (90 - angle)) / 100);	

	float desired_LEFT_RPM;
	float desired_RIGHT_RPM;

	// forward: straight or right turn
	if (angle >= 0 && angle < 90)
	{
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - angle)) / 100.0);
		//				45 - speed 100
		//	1300	650	
		//					1300 * .5 * 1 = 1300 != 650

		leftDir = Forward;
		rightDir = Forward;

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);

		//Serial.println(desired_LEFT_RPM);
		//desired_RIGHT_RPM = MAXRPM * angleAsPercent * (double)(speedRequest / 100);

		//					1300 * .5 = 650
		desired_RIGHT_RPM = desired_LEFT_RPM * angleAsPercent;
	}
	// forward: left turn
	else if (angle < 0 && angle > -90)
	{
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - (angle * -1))) / 100.0);

		leftDir = Forward;
		rightDir = Forward;

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM * angleAsPercent;
	}

	//		  0
	// -90			90
	//		 180

	// backwards: straight or right turn
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

	// backwards: left turn
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

		desired_RIGHT_RPM = desired_LEFT_RPM; 
	}

	//Burn left
	else if (angle == -90)
	{
		//Serial.println("Forward Left");
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - (angle * -1))) / 100.0);

		leftDir = Reverse;
		rightDir = Forward;

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM;	
	}

	// determine slowest wheel for TC purposes
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

void SingleWheelBrake(float currentRPM, float brakeToRPM, float *wheelDuty, Movement currentMovement, Movement *lastWheelDir, int *stopCounter, int *stopDetection, Accel_Or_Decel *currentDirection)
{
	const int minrpm = 2;	   // wheel is stopped if spinning less than this rpm	
	const int jiggleLimit = 2; // if the wheels switch directions this many times, the vehicle is stopped
	const int stopLimit = 2;
	const int rpmReset = 70;   // 70 OK? //20 //170 //lower number causes jittering, higher number causes no brake past low rpms

	//Higher is more brake strength
	const int spinCutterStrength = 4; //4
	const int spinRpm = 5;

	// if wheels are still spinning, brakes will be applied with max power
	if (currentRPM > minrpm)
		*wheelDuty = 100;

	// stop wheels by reversing the direction of the motor
	if (currentMovement == forward) {
		leftDir = Reverse;
	}
	else if (currentMovement == backward) {
		leftDir = Forward;
	}
	// if rpms are 0, count it and set duty to zero
	else if (currentMovement == stopped)
	{
		*stopCounter++;
		*wheelDuty = 0; 
	}

	// count # of direction reversals
	if (*lastWheelDir != currentMovement)
		*stopDetection++;

	// store last wheel movements
	*lastWheelDir = currentMovement;

	// if wheels reverse n times, set duties to 0
	if (*stopDetection >= jiggleLimit)
		*wheelDuty = 0;

	// if rpms are 0 n times or rpms go back up, reset counters
	if (*stopCounter >= stopLimit || currentRPM > rpmReset) {
		*stopDetection = 0;
		*stopCounter = 0;
	}

	//Optimize reverse spin
	if (currentRPM > spinRpm)
	{
		*wheelDuty -= *wheelDuty / spinCutterStrength;

		if (*wheelDuty < 40)
			*wheelDuty = 0;
	}

	*currentDirection = decel;
}

void Brake(RPMS rpm)
{
	const int minrpm = 2;

	// if wheels are still spinning, brakes will be applied with max power
	if (rpm.FL_RPM > minrpm)
		duties.FL_Duty = 100;

	if (rpm.FR_RPM > minrpm)
		duties.FR_Duty = 100;

	if (rpm.BL_RPM > minrpm)
		duties.BL_Duty = 100;

	if (rpm.BR_RPM > minrpm)
		duties.BR_Duty = 100;

	// stop wheels by reversing the direction of the motor
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

	// if the wheels switch directions this many times, the vehicle is stopped
	const int jiggleLimit = 2;

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
	if (rpm.BR_Wheel_movement == stopped)		//Todo these arent adequately triggering
		BR_stopCounter++;
	if (rpm.BL_Wheel_movement == stopped)
		BL_stopCounter++;
	if (rpm.FR_Wheel_movement == stopped)
		FR_stopCounter++;
	if (rpm.FL_Wheel_movement == stopped)
		FL_stopCounter++;

	const int stopLimit = 2;
	const int rpmReset = 70; //70 OK? //20 //170 //lower number causes jittering, higher number causes no brake past low rpms

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
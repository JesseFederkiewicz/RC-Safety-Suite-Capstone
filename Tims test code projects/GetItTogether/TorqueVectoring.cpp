// 
// File: TorqueVectoring.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "TorqueVectoring.h"

const float DUTYMAX = 100.0;
const float DUTYMIN = 50.0; // the lowest duty cycle to spin the motors at, as any less wont turn them
const int MAXRPM = 1300;    // wheel speed limit

bool _firstPass = true;   // for initializing values on the first pass

int _BR_stopDetection = 0;
int _BL_stopDetection = 0;
int _FR_stopDetection = 0;
int _FL_stopDetection = 0;

int _BR_stopCounter = 0;
int _BL_stopCounter = 0;
int _FR_stopCounter = 0;
int _FL_stopCounter = 0;

// the last direction of wheel rotation as determined by the encoder readings/matrix calc
Movement _BR_lastDir = stopped;
Movement _BL_lastDir = stopped;
Movement _FR_lastDir = stopped;
Movement _FL_lastDir = stopped;

MotorDuties _duties;      // duty cycle values to set the motors to
MotorDirection _leftDir;  // direction the left motors will be set to spin
MotorDirection _rightDir; // direction of the right motors

// this function will apply brakes when speed requested is zero, or
// call the steering function to determine duty cycles, 
// and then set motor directions and _duties
ActivedControls Drive(int angle, uint speedRequest, RPMS rpm, int tcLevel, int absLevel)
{
	ActivedControls actives = { false,false,0 };

	if (speedRequest == 0) {
		//Brake(rpm);

		// assign zero desired rpm for full stop
		WheelAndRPMs lf = { rpm.FL_RPM, 0, LF, rpm.FL_Wheel_movement };
		WheelAndRPMs rf = { rpm.FR_RPM, 0, RF, rpm.FR_Wheel_movement };
		WheelAndRPMs lr = { rpm.BL_RPM, 0, LR, rpm.BL_Wheel_movement };
		WheelAndRPMs rr = { rpm.BR_RPM, 0, RR, rpm.BR_Wheel_movement };

		if (SingleWheelBrake(lr, &_duties.BL_Duty, &_BL_lastDir, &_BL_stopCounter, &_BL_stopDetection, absLevel) ||
			SingleWheelBrake(rr, &_duties.BR_Duty, &_BR_lastDir, &_BR_stopCounter, &_BR_stopDetection, absLevel) ||
			SingleWheelBrake(lf, &_duties.FL_Duty, &_FL_lastDir, &_FL_stopCounter, &_FL_stopDetection, absLevel) ||
			SingleWheelBrake(rf, &_duties.FR_Duty, &_FR_lastDir, &_FR_stopCounter, &_FR_stopDetection, absLevel))
			actives.absActivated = true;
	}
	else
	{
		actives = Steering(angle, speedRequest, rpm, tcLevel, absLevel);
		_BR_stopDetection = 0;
		_BL_stopDetection = 0;
		_FR_stopDetection = 0;
		_FL_stopDetection = 0;
	}

	SetMotorDirections(_leftDir, _rightDir);
	SetMotorSpeeds(_duties);

	return actives;
}

// calculates desired rpms for each wheel, based on requested speed and angle, using 
ActivedControls Steering(int angle, uint speedRequest, RPMS rpm, int tcLevel, int absLevel)
{
	//		  0
	// -90			90
	//		 180

	ActivedControls actives = { false,false,0 };
	float desired_LEFT_RPM;
	float desired_RIGHT_RPM;

	// initialize values on first pass
	if (_firstPass)
	{
		_duties.FL_Duty = 0;
		_duties.FR_Duty = 0;
		_duties.BL_Duty = 0;
		_duties.BR_Duty = 0;

		_firstPass = false;
	}

	// forward: straight or right turn
	if (angle >= 0 && angle < 90)
	{
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - angle)) / 100.0);

		_leftDir = Forward;
		_rightDir = Forward;

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_RIGHT_RPM = desired_LEFT_RPM * angleAsPercent;
	}

	// forward: left turn
	else if (angle < 0 && angle > -90)
	{
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - (angle * -1))) / 100.0);

		_leftDir = Forward;
		_rightDir = Forward;

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM * angleAsPercent;
	}

	// backwards: straight or right turn
	else if (angle > 90 && angle <= 180)
	{
		double angleAsPercent = 1 - (((100.0 / 90.0 * (180.0 - angle)) / 100.0));

		_leftDir = Reverse;
		_rightDir = Reverse;

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_RIGHT_RPM = desired_LEFT_RPM * angleAsPercent;
	}

	// backwards: left turn
	else if (angle < -90 && angle > -180)
	{
		double angleAsPercent = 1 - (((100.0 / 90.0 * (180.0 - (angle * -1))) / 100.0));

		_leftDir = Reverse;
		_rightDir = Reverse;

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM * angleAsPercent;
	}

	//Burn right
	if (angle == 90)
	{
		_leftDir = Forward;
		_rightDir = Reverse;

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_RIGHT_RPM = desired_LEFT_RPM;

		actives.burnout = 1;
	}

	//Burn left
	else if (angle == -90)
	{
		_leftDir = Reverse;
		_rightDir = Forward;

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM;

		actives.burnout = 2;
	}

	// determine slowest wheel for TC purposes
	WheelAndRPMs slowestWheel;
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

	WheelAndRPMs backLeft = { rpm.BL_RPM, desired_LEFT_RPM, LR, rpm.BL_Wheel_movement };
	WheelAndRPMs frontLeft = { rpm.FL_RPM, desired_LEFT_RPM, LF, rpm.FL_Wheel_movement };
	WheelAndRPMs backRight = { rpm.BR_RPM, desired_RIGHT_RPM, RR, rpm.BR_Wheel_movement };
	WheelAndRPMs frontRight = { rpm.FR_RPM, desired_RIGHT_RPM, RF, rpm.FR_Wheel_movement };

	if (UpdateDuty(backLeft, &_duties.BL_Duty, slowestWheel, speedRequest, tcLevel, absLevel) ||
		UpdateDuty(frontLeft, &_duties.FL_Duty, slowestWheel, speedRequest, tcLevel, absLevel) ||
		UpdateDuty(backRight, &_duties.BR_Duty, slowestWheel, speedRequest, tcLevel, absLevel) ||
		UpdateDuty(frontRight, &_duties.FR_Duty, slowestWheel, speedRequest, tcLevel, absLevel));
		actives.tcActivated = true;

	return actives;
}

// updates the duty cycle for a wheel based on current rpm, desired rpm, and the state of the slowest wheel if TC is active
bool UpdateDuty(WheelAndRPMs currentWheel, float *duty, WheelAndRPMs slowestWheel, uint speedRequest, int tcLevel, int absLevel)
{
	bool tcActivated = false;

	//Serial.printf("\n\nwheel: %d", currentWheel.wheel);
	//Serial.printf("\ndutyBefore: %f2", *duty);
	//Serial.printf("\nactualRPM: %f2", currentWheel.rpm);
	//Serial.printf("\ndesiredRPM1: %f2", currentWheel.desiredRpm);

	// if traction control is enabled, update desired RPM to match slowest wheel, keeping the ratio for turns
	if (tcLevel && (currentWheel.wheel != slowestWheel.wheel))
	{	
		// used to invert the tcLevel value, so higher val = more traction
		const int maxTC = 11;   

		// the difference between the slowest wheel and the current wheel, needed to activate TC
		float tractionDiff = (MAXRPM * (maxTC - tcLevel) / 20) - 50; 
		/*
			tcLevel - tractionDiff
			----------------------
			  10		15
			  9			80
			  8			145
			  7			210
			  6			270
			  5			340
			  4			405
			  3			470
			  2			535
			  1			600
			  0			n/a
		*/
		if (currentWheel.rpm > slowestWheel.rpm + tractionDiff);
		{
			float ratio = currentWheel.desiredRpm / slowestWheel.desiredRpm;

			currentWheel.desiredRpm = slowestWheel.rpm * ratio;

			// tc was actively changing wheel speeds, send this to database
			tcActivated = true;
		}
	}
	//Serial.printf("\ndesiredRPM2: %f2", currentWheel.desiredRpm);

	// wheel too fast, decrement duty
	if (currentWheel.rpm > currentWheel.desiredRpm)
	{
		if ((*duty -= 10) < 0)
			*duty = 0;
		// insert brakes here instead of regular decrement
		//switch (currentWheel.wheel) 
		//{
		//case LF:
		//	SingleWheelBrake(currentWheel, duty, &_FL_lastDir, &_FL_stopCounter, &_FL_stopDetection, absLevel);
		//	break;
		//case RF:
		//	SingleWheelBrake(currentWheel, duty, &_FR_lastDir, &_FL_stopCounter, &_FL_stopDetection, absLevel);
		//	break;
		//case RR:
		//	SingleWheelBrake(currentWheel, duty, &_BR_lastDir, &_FL_stopCounter, &_FL_stopDetection, absLevel);
		//	break;
		//case LR:
		//	SingleWheelBrake(currentWheel, duty, &_BL_lastDir, &_FL_stopCounter, &_FL_stopDetection, absLevel);
		//	break;
		//}
	}

	// wheel too slow, increment duty
	else if (currentWheel.rpm <= currentWheel.desiredRpm && *duty < 100)
	{
		if (*duty < DUTYMIN)
			*duty = DUTYMIN;

		// sometimes incrementing duty by 1 is too slow, so we'll need to jump up to a higher duty right away
		float guessDuty = currentWheel.desiredRpm * (speedRequest / 1000.0) / (MAXRPM / 1000) / 2 + DUTYMIN - 10;

		if (*duty >= guessDuty)
			*duty += 1;		
		else 
			*duty = guessDuty;		
	}
	//Serial.printf("\ndutyAfter: %f2", *duty);
	return tcActivated;
}

// applies braking to a single motor
bool SingleWheelBrake(WheelAndRPMs wheel, float* wheelDuty, Movement* lastWheelDir,
						int* stopCounter, int* stopDetection, int absLevel)
{
	bool absActivated = false;
	const int jiggleLimit = 2; // if the wheels switch directions this many times, the vehicle is stopped
	const int stopLimit = 2;
	const int rpmReset = 70;   // 70 OK? //20 //170 //lower number causes jittering, higher number causes no brake past low rpms

	//Higher is more brake strength
	const int spinCutterStrength = 4;
	const int spinRpm = 5;

	// no abs, apply brakes with full power
	if (!absLevel && wheel.rpm > wheel.desiredRpm)
		*wheelDuty = DUTYMAX;

	// abs active, use transfer function to give less duty as wheels slow down
	else if(absLevel && wheel.rpm > wheel.desiredRpm)
	{
		// transfer function of RPM to duty of 50-100%
		*wheelDuty = wheel.rpm * ((DUTYMAX - DUTYMIN) / (MAXRPM - wheel.desiredRpm)) + DUTYMIN;
	}	

	// stop wheels by reversing the direction of the motor
	switch (wheel.wheel)
	{
	case LR:
	case LF:
		if (wheel.movement == forward) {
			_leftDir = Reverse;
		}
		else if (wheel.movement == backward) {
			_leftDir = Forward;
		}
		break;

	case RF:
	case RR:
		if (wheel.movement == forward) {
			_rightDir = Reverse;
		}
		else if (wheel.movement == backward) {
			_rightDir = Forward;
		}
		break;
	}

	// if rpms are 0, count it 
	if (wheel.movement == stopped)
		*stopCounter += 1;

	// count # of direction reversals
	if (*lastWheelDir != wheel.movement)
		*stopDetection += 1;

	// store last wheel movements
	*lastWheelDir = wheel.movement;

	// if wheels reverse n times, set _duties to 0
	if (*stopDetection >= jiggleLimit)
		*wheelDuty = 0;

	// if rpms are 0 n times or rpms go back up, reset counters
	if (*stopCounter >= stopLimit || wheel.rpm > rpmReset) 
	{
		*stopDetection = 0;
		*stopCounter = 0;
	}

	//Optimize reverse spin
	if (wheel.rpm > spinRpm)
	{
		*wheelDuty -= *wheelDuty / spinCutterStrength;

		if (*wheelDuty < 40)
			*wheelDuty = 0;
	}
	return absActivated;
}

//void Brake(RPMS rpm)
//{
//	const int minrpm = 2;
//
//	// if wheels are still spinning, brakes will be applied with max power
//	if (rpm.FL_RPM > minrpm)
//		_duties.FL_Duty = 100;
//
//	if (rpm.FR_RPM > minrpm)
//		_duties.FR_Duty = 100;
//
//	if (rpm.BL_RPM > minrpm)
//		_duties.BL_Duty = 100;
//
//	if (rpm.BR_RPM > minrpm)
//		_duties.BR_Duty = 100;
//
//	// stop wheels by reversing the direction of the motor
//	if (rpm.BL_Wheel_movement == forward) {
//		_leftDir = Reverse;
//	}
//	else if (rpm.BL_Wheel_movement == backward) {
//		_leftDir = Forward;
//	}
//
//	if (rpm.FL_Wheel_movement == forward) {
//		_leftDir = Reverse;
//	}
//	else if (rpm.FL_Wheel_movement == backward) {
//		_leftDir = Forward;
//	}
//
//	if (rpm.FR_Wheel_movement == forward) {
//		_rightDir = Reverse;
//	}
//	else if (rpm.FR_Wheel_movement == backward) {
//		_rightDir = Forward;
//	}
//
//	if (rpm.BR_Wheel_movement == forward) {
//		_rightDir = Reverse;
//	}
//	else if (rpm.BR_Wheel_movement == backward) {
//		_rightDir = Forward;
//	}
//
//	// count # of direction reversals
//	if (_BR_lastDir != rpm.BR_Wheel_movement)
//		_BR_stopDetection++;
//
//	if (_BL_lastDir != rpm.BL_Wheel_movement)
//		_BL_stopDetection++;
//
//	if (_FR_lastDir != rpm.FR_Wheel_movement)
//		_FR_stopDetection++;
//
//	if (_FL_lastDir != rpm.FL_Wheel_movement)
//		_FL_stopDetection++;
//
//	// store last wheel movements
//	_BR_lastDir = rpm.BR_Wheel_movement;
//	_BL_lastDir = rpm.BL_Wheel_movement;
//	_FR_lastDir = rpm.FR_Wheel_movement;
//	_FL_lastDir = rpm.FL_Wheel_movement;
//
//	// if the wheels switch directions this many times, the vehicle is stopped
//	const int jiggleLimit = 2;
//
//	// if wheels reverse n times, set _duties to 0
//	if (_BR_stopDetection >= jiggleLimit)
//		_duties.BR_Duty = 0;
//	if (_BL_stopDetection >= jiggleLimit)
//		_duties.BL_Duty = 0;
//	if (_FR_stopDetection >= jiggleLimit)
//		_duties.FR_Duty = 0;
//	if (_FL_stopDetection >= jiggleLimit)
//		_duties.FL_Duty = 0;
//
//	// if rpms are 0, count it
//	if (rpm.BR_Wheel_movement == stopped)		//Todo these arent adequately triggering
//		_BR_stopCounter++;
//	if (rpm.BL_Wheel_movement == stopped)
//		_BL_stopCounter++;
//	if (rpm.FR_Wheel_movement == stopped)
//		_FR_stopCounter++;
//	if (rpm.FL_Wheel_movement == stopped)
//		_FL_stopCounter++;
//
//	const int stopLimit = 2;
//	const int rpmReset = 70; //70 OK? //20 //170 //lower number causes jittering, higher number causes no brake past low rpms
//
//	//focus area ^ 
//
//	// if rpms are 0 n times, reset counters
//	if (_BR_stopCounter >= stopLimit || rpm.BR_RPM > rpmReset) {
//		_BR_stopDetection = 0;
//		_BR_stopCounter = 0;
//	}
//	if (_BL_stopCounter >= stopLimit || rpm.BL_RPM > rpmReset) {
//		_BL_stopDetection = 0;
//		_BL_stopCounter = 0;
//	}
//	if (_FR_stopCounter >= stopLimit || rpm.FR_RPM > rpmReset) {
//		_FR_stopDetection = 0;
//		_FR_stopCounter = 0;
//	}
//	if (_FL_stopCounter >= stopLimit || rpm.FL_RPM > rpmReset) {
//		_FL_stopDetection = 0;
//		_FL_stopCounter = 0;
//	}
//
//	//Higher is more brake strength
//	int spinCutterStrength = 4; //4
//	int spinRpm = 5;
//
//	//Optimize reverse spin
//	if (rpm.BR_RPM > spinRpm)
//	{
//		_duties.BR_Duty -= _duties.BR_Duty / spinCutterStrength;
//
//		if (_duties.BR_Duty < 40)
//			_duties.BR_Duty = 0;
//	}
//
//	if (rpm.BL_RPM > spinRpm)
//	{
//		_duties.BL_Duty -= _duties.BL_Duty / spinCutterStrength;
//
//		if (_duties.BL_Duty < 40)
//			_duties.FL_Duty = 0;
//	}
//
//	if (rpm.FR_RPM > spinRpm)
//	{
//		_duties.FR_Duty -= _duties.FR_Duty / spinCutterStrength;
//
//		if (_duties.FL_Duty < 40)
//			_duties.FL_Duty = 0;
//	}
//
//	if (rpm.FL_RPM > spinRpm)
//	{
//		_duties.FL_Duty -= _duties.FL_Duty / spinCutterStrength;
//
//		if (_duties.FL_Duty < 40)
//			_duties.FL_Duty = 0;
//	}
//
//	_curWheelDirection.BackLeft = decel;
//	_curWheelDirection.BackRight = decel;
//	_curWheelDirection.FrontLeft = decel;
//	_curWheelDirection.FrontRight = decel;
//}
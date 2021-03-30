// 
// File: TorqueVectoring.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// Description: contains functions that allow the vehicle to drive,
//				 sets motor duties and directions based on current inputs and feedback data
// 

#include "TorqueVectoring.h"

const float DUTYMIN = 50.0;  // the lowest duty cycle to spin the motors at, as any less wont turn them
const float DUTYMAX = 100.0;
const int MAXRPM = 1300;     // wheel speed limit

bool _firstPass = true;   // for initializing values on start up

// values used for determining when the vehicle is stopped during braking
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
ActiveControls Drive(int angle, uint speedRequest, RPMS rpm, int tcLevel, int absLevel)
{
	ActiveControls actives = { false,false,0 }; // values to return to main function

	// brakes requested
	if (speedRequest == 0) 
	{
		// wheel info to pass to brake function, assign zero desired rpm for full stop
		WheelAndRPMs lf = { rpm.FL_RPM, 0, LF, rpm.FL_Wheel_movement };
		WheelAndRPMs rf = { rpm.FR_RPM, 0, RF, rpm.FR_Wheel_movement };
		WheelAndRPMs lr = { rpm.BL_RPM, 0, LR, rpm.BL_Wheel_movement };
		WheelAndRPMs rr = { rpm.BR_RPM, 0, RR, rpm.BR_Wheel_movement };

		// call brake function for each motor independantly

		if (SingleWheelBrake(lr, &_duties.BL_Duty, &_BL_lastDir, &_BL_stopCounter, &_BL_stopDetection, absLevel))
			actives.absActivated = true;
		if (SingleWheelBrake(rr, &_duties.BR_Duty, &_BR_lastDir, &_BR_stopCounter, &_BR_stopDetection, absLevel))
			actives.absActivated = true;
		if (SingleWheelBrake(lf, &_duties.FL_Duty, &_FL_lastDir, &_FL_stopCounter, &_FL_stopDetection, absLevel))
			actives.absActivated = true;
		if (SingleWheelBrake(rf, &_duties.FR_Duty, &_FR_lastDir, &_FR_stopCounter, &_FR_stopDetection, absLevel))
			actives.absActivated = true;
	}
	// driving requested
	else
	{
		actives = Steering(angle, speedRequest, rpm, tcLevel, absLevel);

		// reset stop values when driving
		_BR_stopDetection = 0;
		_BL_stopDetection = 0;
		_FR_stopDetection = 0;
		_FL_stopDetection = 0;
	}

	// set duties and direction as per brakes/steering
	SetMotorDirections(_leftDir, _rightDir);
	SetMotorSpeeds(_duties);

	return actives;
}

// calculates desired rpms for each wheel, based on requested speed and angle, 
//	then calls UpdateDuty for each wheel to get actual duty cylce required to hit desired RPM
ActiveControls Steering(int angle, uint speedRequest, RPMS rpm, int tcLevel, int absLevel)
{
	ActiveControls actives = { false,false,0 }; // active systems to pass back to main

	// desired rpm values for each side of the vehicle
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
	// right wheels slower to turn right
	if (angle >= 0 && angle < 90)
	{
		_leftDir = Forward;
		_rightDir = Forward;

		// use angle up to 90 degrees as a percent/100
		double angleAsPercent = ((100.0 / 90.0 * (90.0 - angle)) / 100.0);

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_RIGHT_RPM = desired_LEFT_RPM * angleAsPercent;
	}

	// forward: left turn
	// left wheels slower to turn left
	else if (angle < 0 && angle > -90)
	{
		_leftDir = Forward;
		_rightDir = Forward;

		double angleAsPercent = ((100.0 / 90.0 * (90.0 - (angle * -1))) / 100.0);

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM * angleAsPercent;
	}

	// backwards: straight or right turn
	// right wheels slower to turn right
	else if (angle > 90 && angle <= 180)
	{
		_leftDir = Reverse;
		_rightDir = Reverse;

		double angleAsPercent = 1 - (((100.0 / 90.0 * (180.0 - angle)) / 100.0));

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_RIGHT_RPM = desired_LEFT_RPM * angleAsPercent;
	}

	// backwards: left turn
	// left wheels slower to turn left
	else if (angle < -90 && angle > -180)
	{
		_leftDir = Reverse;
		_rightDir = Reverse;

		double angleAsPercent = 1 - (((100.0 / 90.0 * (180.0 - (angle * -1))) / 100.0));

		desired_RIGHT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_LEFT_RPM = desired_RIGHT_RPM * angleAsPercent;
	}

	// Burn right
	// wheels turn in opposite directions
	if (angle == 90)
	{
		_leftDir = Forward;
		_rightDir = Reverse;

		desired_LEFT_RPM = MAXRPM * (speedRequest / 100.0);
		desired_RIGHT_RPM = desired_LEFT_RPM;

		actives.burnout = 1;
	}

	// Burn left
	// wheels turn in opposite directions
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

	// wheel info to be used for setting duties 
	WheelAndRPMs backLeft = { rpm.BL_RPM, desired_LEFT_RPM, LR, rpm.BL_Wheel_movement };
	WheelAndRPMs frontLeft = { rpm.FL_RPM, desired_LEFT_RPM, LF, rpm.FL_Wheel_movement };
	WheelAndRPMs backRight = { rpm.BR_RPM, desired_RIGHT_RPM, RR, rpm.BR_Wheel_movement };
	WheelAndRPMs frontRight = { rpm.FR_RPM, desired_RIGHT_RPM, RF, rpm.FR_Wheel_movement };

	if (UpdateDuty(backLeft, &_duties.BL_Duty, slowestWheel, speedRequest, tcLevel, absLevel))
		actives.tcActivated = true;
	if (UpdateDuty(frontLeft, &_duties.FL_Duty, slowestWheel, speedRequest, tcLevel, absLevel))
		actives.tcActivated = true;
	if (UpdateDuty(backRight, &_duties.BR_Duty, slowestWheel, speedRequest, tcLevel, absLevel))
		actives.tcActivated = true;
	if (UpdateDuty(frontRight, &_duties.FR_Duty, slowestWheel, speedRequest, tcLevel, absLevel))
		actives.tcActivated = true;

	return actives;
}

// updates the duty cycle for a wheel based on current rpm, desired rpm, and the state of the slowest wheel if TC is active
bool UpdateDuty(WheelAndRPMs currentWheel, float *duty, WheelAndRPMs slowestWheel, uint speedRequest, int tcLevel, int absLevel)
{
	bool tcActivated = false; // active value to be passed back to main

	// if traction control is enabled, update desired RPM to match slowest wheel, keeping the ratio for turns
	if (tcLevel && (currentWheel.wheel != slowestWheel.wheel))
	{	
		// used to invert the tcLevel value, so higher val = more traction
		const int maxTC = 11;   

		// the max difference in rpm between the slowest wheel and the current wheel to activate TC
		float tractionDiff = (MAXRPM * (maxTC - tcLevel) / 20) - 50; 
		/*
			tcLevel - tractionDiff (rpm)
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

		// if current wheel rpm is out of range of the slowest wheel rpm
		if (currentWheel.rpm > slowestWheel.rpm + tractionDiff);
		{
			// update the desired rpm based on the slowest wheel, keeping the same turn ratio
			float ratio = currentWheel.desiredRpm / slowestWheel.desiredRpm;
			currentWheel.desiredRpm = slowestWheel.rpm * ratio;

			// tc was actively changing motor duties, send this to database
			tcActivated = true;
		}
	}

	// wheel too fast, decrement duty
	if (currentWheel.rpm > currentWheel.desiredRpm)
	{
		if ((*duty -= 10) < 0)
			*duty = 0;
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

	return tcActivated;
}

// applies braking to a single motor, either locking or using abs, and with a duty strength based on absLevel
bool SingleWheelBrake(WheelAndRPMs wheel, float* wheelDuty, Movement* lastWheelDir,
						int* stopCounter, int* stopDetection, int absLevel)
{
	bool absActivated = false; // active to pass back to main
	const int jiggleLimit = 2; // if the wheels switch directions this many times, the vehicle is stopped
	const int stopLimit = 2;
	const int rpmReset = 70;   // 70 OK? //20 //170 //lower number causes jittering, higher number causes no brake past low rpms
 
	//Higher is more brake strength
	const int spinCutterStrength = 4;
	const int spinRpm = 5;

	// used to invert the absLevel value, so higher val = stricter ABS
	const int maxABS = 11;

	// no abs, apply brakes with full power if still spinning
	if (!absLevel && wheel.rpm > wheel.desiredRpm)
		*wheelDuty = DUTYMAX;

	// abs active, 
	else if (absLevel && wheel.rpm > wheel.desiredRpm + 10)
	{
		//float temp = wheel.rpm * ((DUTYMAX - DUTYMIN) / (MAXRPM - wheel.desiredRpm)) + DUTYMIN + (absLevel * 5);
		float temp = DUTYMAX + 5 - ((maxABS - absLevel) * 5);
		*wheelDuty = temp;
		absActivated = true;
	}
	else if(absLevel)
	{
		*wheelDuty = 0;
		absActivated = true;
	}
	Serial.printf("\nduty: %f2", *wheelDuty);

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
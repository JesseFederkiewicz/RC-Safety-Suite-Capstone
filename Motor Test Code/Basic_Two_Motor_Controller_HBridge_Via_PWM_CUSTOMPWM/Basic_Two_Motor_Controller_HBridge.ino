/*
 Name:		Basic_Two_Motor_Controller_HBridge.ino
 Created:	1/22/2021 4:44:22 PM
 Author:	Jesse Federkiewicz
*/

// the setup function runs once when you press reset or power the board
#include "pwm.h"
#include "Functions.h"
void setup() {
	setupFuncTestCpp();
}

// the loop function runs over and over again until power down or reset
void loop() {
	motorLogicCpp();
}

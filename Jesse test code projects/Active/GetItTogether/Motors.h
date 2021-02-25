// Motors.h

#ifndef _MOTORS_h
#define _MOTORS_h

#include "driver/mcpwm.h"
#include "driver/gpio.h"
#include "HardwareSerial.h"

// custom structs to hold several emuns that are needed for configs/inits/setting duty:
typedef struct PWM_Settings {
	int pin;                   // the GPIO pin for the pwm signal going to the H-bridge and motor
	mcpwm_unit_t unit;         // the pwm unit that will control the motor
	mcpwm_timer_t timer;       // the timer to use for the pwm operator
	mcpwm_operator_t opOut;    // the pwm operator output to use with this motor
	mcpwm_io_signals_t signal; // the specific pwm io signal
};

typedef enum MotorDirection {
	Forward = 0,
	Reverse = 1
};

typedef enum Movement {
	forward,
	backward,
	stopped
};

typedef struct MotorDuties
{
	float FL_Duty;
	float FR_Duty;
	float BL_Duty;
	float BR_Duty;
};

void InitMotors();
void SetMotorSpeeds(MotorDuties duties);
void SetMotorDirections(MotorDirection leftDirection, MotorDirection rightDirection);

#endif
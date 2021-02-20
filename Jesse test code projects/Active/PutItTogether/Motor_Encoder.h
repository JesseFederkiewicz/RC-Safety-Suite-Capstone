// Encoders.h

#ifndef _ENCODERS_h
#define _ENCODERS_h

#include "driver/mcpwm.h"
#include "esp32-hal.h"
#include "driver/gpio.h"
#include "HardwareSerial.h"
#include "driver/timer.h"
#include "driver/pcnt.h"
#include "arduino.h"

#include "driver/mcpwm.h"
#include "driver/gpio.h"
#include "HardwareSerial.h"

//// custom structs to hold several emuns that are needed for configs/inits/setting duty:
//typedef struct PWM_Settings {
//	int pin;                   // the GPIO pin for the pwm signal going to the H-bridge and motor
//	mcpwm_unit_t unit;         // the pwm unit that will control the motor
//	mcpwm_timer_t timer;       // the timer to use for the pwm operator
//	mcpwm_operator_t opOut;    // the pwm operator output to use with this motor
//	mcpwm_io_signals_t signal; // the specific pwm io signal
//};

const int _timerPrescale = 80;
const int _timerClk = 80000000 / _timerPrescale;
const int _intTriggerPeriod_ms = 50;

typedef enum MotorDirection {
	Forward = 0,
	Reverse = 1
};

typedef struct MotorDuties
{
	float FL_Duty;
	float FR_Duty;
	float BL_Duty;
	float BR_Duty;
};

void SetMotorSpeeds(MotorDuties duties);
void SetMotorDirections(MotorDirection leftDirection, MotorDirection rightDirection);


typedef struct RPMS {
	float FL_RPM;
	float FR_RPM;
	float BL_RPM;
	float BR_RPM;
};

// custom structs to hold several emuns that are needed for configs/inits/setting duty:
typedef struct PWM_Settings {
	int pin;                   // the GPIO pin for the pwm signal going to the H-bridge and motor
	mcpwm_unit_t unit;         // the pwm unit that will control the motor
	mcpwm_timer_t timer;       // the timer to use for the pwm operator
	mcpwm_operator_t opOut;    // the pwm operator output to use with this motor
	mcpwm_io_signals_t signal; // the specific pwm io signal
};

typedef struct Encoder_Settings {
	int pin;                             // GPIO pin of the encoder input signal
	gpio_num_t gpioNum;                  // gpio num
	mcpwm_capture_on_edge_t edgeCapture; // which edge detection to use, pos/neg
};

// bundle up each struct into a nice group for each motor
typedef struct Motor_Settings {
	PWM_Settings pwm;
	Encoder_Settings encoder;
};


void MotorAndEncoderInits();
RPMS GetRPMS();


#endif


/*
 Name:			PWM_Code.cpp
 Created:		1/22/2021
 Last Modified:
 Author:		Tim Hachey & Jesse Federkiewicz
 Description:	Arduino first test library...
				quite the learning curve up in here
*/


/*

Puropse of this document is to determine what control we have over the motor as its operating range may not actually be 0-12V,
One motor will be tested for the duty its not moving, the other will be hooked to a dmm to determine the voltage needed to move withload any ground resistance

Findings:
TBD - Jan 25 - need new board coming in wednesday


*/
#include "PWM_Code.h"
#include "driver/mcpwm.h"
#include "esp32-hal.h"
#include "HardwareSerial.h"
#include "driver/gpio.h"

// custom struct to hold several emuns that are needed for config/init/setting duty
typedef struct {
	mcpwm_unit_t unit;
	mcpwm_timer_t timer;
	mcpwm_operator_t opOut;
	mcpwm_io_signals_t pwmSignal;
	mcpwm_io_signals_t encoderSignal;
	int pwmPin;
	int encoderPin;
	
}PWM_Motor_Settings;

// custom set duty function so we can pass one struct and duty
void SetDuty(PWM_Motor_Settings motor, float duty) {
	mcpwm_set_duty(motor.unit, motor.timer, motor.opOut, duty);
}

void Main() {
	Serial.begin(9600);
	// motor one PWM channel settings
	PWM_Motor_Settings m1;
	m1.unit = MCPWM_UNIT_0;   // m1 using pwm unit 0
	m1.timer = MCPWM_TIMER_0; // m1 using unit 0 timer 0
	m1.opOut = MCPWM_OPR_A;   // m1 on operator 0 output A
	m1.pwmSignal = MCPWM0A;      // sort of the same as above, needed for gpio_init
	m1.pwmPin = 22;          // pwm for m1 on pin 22
	m1.encoderPin = 34;

	// motor two PWM channel settings
	PWM_Motor_Settings m2;
	m2.unit = m1.unit;      // m2 shares unit with m1
	m2.timer = m1.timer;    // m2 shares timer with m1
	m2.opOut = MCPWM_OPR_B; // m2 on operator 0 output B
	m2.pwmSignal = MCPWM0B; // needed for gpio_init
	m2.pwmPin = 23;			// pwm for m1 on pin 22
	m2.encoderPin = 35;

	// config for pwm unit 0 timer 0 
	mcpwm_config_t pwmconf;
	pwmconf.frequency = 10000;				 // timer freq 10k
	pwmconf.duty_mode = MCPWM_DUTY_MODE_0;   // positive duty cycle
	pwmconf.counter_mode = MCPWM_UP_COUNTER; // timer up counter
	pwmconf.cmpr_a = 0;						 // initial duty of 0
	pwmconf.cmpr_b = 0;

	// init pwm module
	mcpwm_gpio_init(m1.unit, m1.pwmSignal, m1.pwmPin);
	mcpwm_gpio_init(m2.unit, m2.pwmSignal, m2.pwmPin);
	mcpwm_init(m1.unit, m1.timer, &pwmconf);



	// main loop
	for (;;)
	{
		// ramp rpm up
		for (int dutyCycle = 0; dutyCycle <= 100; dutyCycle++) {
			SetDuty(m1, dutyCycle);
			SetDuty(m2, dutyCycle);
			//SetDuty(m2, 100 - dutyCycle);
			Serial.println(dutyCycle);
			delay(200);
		}
		delay(1000);

		// rpm down
		for (int dutyCycle = 100; dutyCycle >= 0; dutyCycle--) {
			SetDuty(m1, dutyCycle);
			SetDuty(m2, dutyCycle);
			//SetDuty(m2, 100 - dutyCycle);
			Serial.println(dutyCycle);
			delay(200);
		}
		delay(1000);
	}
}

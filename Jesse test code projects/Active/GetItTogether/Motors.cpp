// 
// File: Motors.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// Description: contains functions for initializing the motors
//				and setting duty cycles and directions for each motor
// 

#include "Motors.h"

// config structs, used for inits and setting duties
PWM_Settings FL_PWM;
PWM_Settings FR_PWM;
PWM_Settings BL_PWM;
PWM_Settings BR_PWM;

// gpio number of the 2 directional control pins, used for inits and setting directions
gpio_num_t rightDirControlPin;
gpio_num_t leftDirControlPin;

void InitMotors() 
{
	// motor one configs
	FL_PWM.unit = MCPWM_UNIT_0;   // Front Left using pwm unit 0
	FL_PWM.timer = MCPWM_TIMER_0; // Front Left using unit 0 timer 0
	FL_PWM.opOut = MCPWM_OPR_A;   // Front Left on operator 0 output A
	FL_PWM.signal = MCPWM0A;      // sort of the same as above, needed for gpio_init
	FL_PWM.pin = 22;              // pwm for Front Left on pin 22

	// motor two configs
	FR_PWM.unit = FL_PWM.unit;   // Front Right shares unit with FL_PWM
	FR_PWM.timer = FL_PWM.timer; // Front Right shares timer with FL_PWM
	FR_PWM.opOut = MCPWM_OPR_B;  // Front Right on operator 0 output B
	FR_PWM.signal = MCPWM0B;     // needed for gpio_init
	FR_PWM.pin = 23;		     // pwm for Front Right on pin 23

	// motor three configs
	BL_PWM.unit = MCPWM_UNIT_1;   // Back Left using pwm unit 1
	BL_PWM.timer = MCPWM_TIMER_0; // Back Left using unit 1 timer 0
	BL_PWM.opOut = MCPWM_OPR_A;   // Back Left on operator 0 output A
	BL_PWM.signal = MCPWM0A;      // needed for gpio_init
	BL_PWM.pin = 12;		      // pwm for Back Left on pin 12

	// motor four configs
	BR_PWM.unit = BL_PWM.unit;   // Back Right shares unit with Back Left
	BR_PWM.timer = BL_PWM.timer; // Back Right shares timer with Back Left
	BR_PWM.opOut = MCPWM_OPR_B;  // Back Right on operator 0 output B
	BR_PWM.signal = MCPWM0B;     // needed for gpio_init
	BR_PWM.pin = 2;			     // pwm for Back Right on pin 2

	// set GPIOs for PWMs
	mcpwm_gpio_init(FL_PWM.unit, FL_PWM.signal, FL_PWM.pin);
	mcpwm_gpio_init(FR_PWM.unit, FR_PWM.signal, FR_PWM.pin);
	mcpwm_gpio_init(BL_PWM.unit, BL_PWM.signal, BL_PWM.pin);
	mcpwm_gpio_init(BR_PWM.unit, BR_PWM.signal, BR_PWM.pin);

	// config/init each PWM unit
	mcpwm_config_t pwmconf;
	pwmconf.frequency = 10000;				 // timer freq 10k
	pwmconf.duty_mode = MCPWM_DUTY_MODE_0;   // positive duty cycle
	pwmconf.counter_mode = MCPWM_UP_COUNTER; // timer up counter
	pwmconf.cmpr_a = 0;						 // initial duty of 0
	pwmconf.cmpr_b = 0;

	mcpwm_init(FL_PWM.unit, FL_PWM.timer, &pwmconf);
	mcpwm_init(BL_PWM.unit, BL_PWM.timer, &pwmconf);

	// Init GPIOs for diectional control

	// setup gpio pin 21 for rightDirection control
	rightDirControlPin = GPIO_NUM_21;
	gpio_config_t rightDirConfig;
	rightDirConfig.intr_type = GPIO_INTR_DISABLE;
	rightDirConfig.mode = GPIO_MODE_OUTPUT;
	rightDirConfig.pin_bit_mask = (0b01 << 21); // bit #21 for pin 21
	rightDirConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
	rightDirConfig.pull_up_en = GPIO_PULLUP_ENABLE;

	// setup gpio pin 4 for leftDirection control
	leftDirControlPin = GPIO_NUM_4;
	gpio_config_t leftDirConfig;
	leftDirConfig.intr_type = GPIO_INTR_DISABLE;
	leftDirConfig.mode = GPIO_MODE_OUTPUT;
	leftDirConfig.pin_bit_mask = 0b10000; // bit #4 for pin 4
	leftDirConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
	leftDirConfig.pull_up_en = GPIO_PULLUP_ENABLE;

	// init pins
	gpio_config(&rightDirConfig);
	gpio_config(&leftDirConfig);

	// set direction Forward on start
	SetMotorDirections(Forward, Forward);
}

// sets all duties for each motor
void SetMotorSpeeds(MotorDuties duties)
{
	mcpwm_set_duty(FL_PWM.unit, FL_PWM.timer, FL_PWM.opOut, duties.FL_Duty);
	mcpwm_set_duty(FR_PWM.unit, FR_PWM.timer, FR_PWM.opOut, duties.FR_Duty);
	mcpwm_set_duty(BL_PWM.unit, BL_PWM.timer, BL_PWM.opOut, duties.BL_Duty);
	mcpwm_set_duty(BR_PWM.unit, BR_PWM.timer, BR_PWM.opOut, duties.BR_Duty);
}

// sets all directions for both sets of motors
void SetMotorDirections(MotorDirection leftDirection, MotorDirection rightDirection) 
{
	gpio_set_level(leftDirControlPin, leftDirection);
	gpio_set_level(rightDirControlPin, rightDirection);
}
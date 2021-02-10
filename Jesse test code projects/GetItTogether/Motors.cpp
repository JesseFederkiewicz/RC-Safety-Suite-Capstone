// 
// 
// 

#include "Motors.h"

// config structs
PWM_Settings FL_PWM;
PWM_Settings FR_PWM;
PWM_Settings BL_PWM;
PWM_Settings BR_PWM;

gpio_num_t rightDirControlPin;
gpio_num_t leftDirControlPin;

void InitMotors() 
{
	// motor one configs
	FL_PWM.unit = MCPWM_UNIT_0;              // FL_PWM using pwm unit 0
	FL_PWM.timer = MCPWM_TIMER_0;            // FL_PWM using unit 0 timer 0
	FL_PWM.opOut = MCPWM_OPR_A;              // FL_PWM on operator 0 output A
	FL_PWM.signal = MCPWM0A;                 // sort of the same as above, needed for gpio_init
	FL_PWM.pin = 22;                         // pwm for FL_PWM on pin 22

	// motor two configs
	FR_PWM.unit = FL_PWM.unit;   // FR_PWM shares unit with FL_PWM
	FR_PWM.timer = FL_PWM.timer; // FR_PWM shares timer with FL_PWM
	FR_PWM.opOut = MCPWM_OPR_B;              // FR_PWM on operator 0 output B
	FR_PWM.signal = MCPWM0B;                 // needed for gpio_init
	FR_PWM.pin = 23;		                  // pwm for FL_PWM on pin 23

	// motor three configs
	BL_PWM.unit = MCPWM_UNIT_1;              // FR_PWM shares unit with FL_PWM
	BL_PWM.timer = MCPWM_TIMER_0;            // FR_PWM shares timer with FL_PWM
	BL_PWM.opOut = MCPWM_OPR_A;              // FR_PWM on operator 0 output B
	BL_PWM.signal = MCPWM0A;                 // needed for gpio_init
	BL_PWM.pin = 15;		                    // pwm for FL_PWM on pin 15

	// motor four configs
	BR_PWM.unit = BL_PWM.unit;    // FR_PWM shares unit with FL_PWM
	BR_PWM.timer = BL_PWM.timer;  // FR_PWM shares timer with FL_PWM
	BR_PWM.opOut = MCPWM_OPR_B;              // FR_PWM on operator 0 output B
	BR_PWM.signal = MCPWM0B;                 // needed for gpio_init
	BR_PWM.pin = 2;			                 // pwm for FL_PWM on pin 2

	// run PWM setup for each motor
	mcpwm_gpio_init(FL_PWM.unit, FL_PWM.signal, FL_PWM.pin);
	mcpwm_gpio_init(FR_PWM.unit, FR_PWM.signal, FR_PWM.pin);
	mcpwm_gpio_init(BL_PWM.unit, BL_PWM.signal, BL_PWM.pin);
	mcpwm_gpio_init(BR_PWM.unit, BR_PWM.signal, BR_PWM.pin);

	// config/ init each PWM unit
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
	rightDirConfig.pin_bit_mask = 0b1000000000000000000000; // bit #21 for pin 21
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

	gpio_config(&rightDirConfig);
	gpio_config(&leftDirConfig);

	// set direction Forward for now
	SetMotorDirections(Forward, Forward);
}

void SetMotorSpeeds(MotorDuties duties)
{
	//Set motor duty
	mcpwm_set_duty(FL_PWM.unit, FL_PWM.timer, FL_PWM.opOut, duties.FL_Duty);
	mcpwm_set_duty(FR_PWM.unit, FR_PWM.timer, FR_PWM.opOut, duties.FR_Duty);
	mcpwm_set_duty(BL_PWM.unit, BL_PWM.timer, BL_PWM.opOut, duties.BL_Duty);
	mcpwm_set_duty(BR_PWM.unit, BR_PWM.timer, BR_PWM.opOut, duties.BR_Duty);
}

void SetMotorDirections(MotorDirection leftDirection, MotorDirection rightDirection) 
{
	//Serial.printf("setting direction\n");
	//Serial.printf("gpio %d = %d\n", leftDirControlPin, leftDirection);
	//Serial.printf("gpio %d = %d\n", rightDirControlPin, rightDirection);
	gpio_set_level(leftDirControlPin, leftDirection);
	gpio_set_level(rightDirControlPin, rightDirection);
}
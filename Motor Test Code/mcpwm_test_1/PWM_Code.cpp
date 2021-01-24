/*
 Name:		  PWM_Code.cpp
 Created:	  1/24/2021
 Author:	  Tim Hachey
 Description: first use of the mcpwm module on the esp32
*/

#include "PWM_Code.h"
#include "driver/pcnt.h"
#include "driver/mcpwm.h"
#include "esp32-hal.h"
#include "HardwareSerial.h"

// custom struct to hold several emuns that are needed for config/init/setting duty
typedef struct {
	mcpwm_unit_t unit;
	mcpwm_timer_t timer;
	mcpwm_operator_t opOut;
	mcpwm_io_signals_t signal;
}PWM_Motor;

// custom set duty function so we can pass one struct and duty
void SetDuty(PWM_Motor motor, float duty) {
	mcpwm_set_duty(motor.unit, motor.timer, motor.opOut, duty);
}

void Main() {

	Serial.begin(115200);

	// pwm pins
	const int motorOnePin = 22;  // GPIO22
	const int motorTwoPin = 23;  // GPIO23

	// motor one PWM channel settings
	PWM_Motor m1;
	m1.unit = MCPWM_UNIT_0;   // m1 using pwm unit 0
	m1.timer = MCPWM_TIMER_0; // m1 using unit 0 timer 0
	m1.opOut = MCPWM_OPR_A;   // m1 on operator 0 output A
	m1.signal = MCPWM0A;      // sort of the same as above, needed for gpio_init

	// motor two PWM channel settings
	PWM_Motor m2;
	m2.unit = m1.unit;      // m2 shares unit with m1
	m2.timer = m1.timer;    // m2 shares timer with m1
	m2.opOut = MCPWM_OPR_B; // m2 on operator 0 output B
	m2.signal = MCPWM0B;    // needed for gpio_init

	// config for pwm unit 0 timer 0 
	mcpwm_config_t pwmconf;
	pwmconf.frequency = 10000;				 // timer freq 10k
	pwmconf.duty_mode = MCPWM_DUTY_MODE_0;   // positive duty cycle
	pwmconf.counter_mode = MCPWM_UP_COUNTER; // timer up counter
	pwmconf.cmpr_a = 0;						 // initial duty of 0
	pwmconf.cmpr_b = 0;

	// init pwm module
	mcpwm_gpio_init(m1.unit, m1.signal, motorOnePin);
	mcpwm_gpio_init(m2.unit, m2.signal, motorTwoPin);
	mcpwm_init(m1.unit, m1.timer, &pwmconf);

	// main loop
	for (;;) {

		// ramp rpm up
		for (int dutyCycle = 0; dutyCycle <= 100; dutyCycle++) {
			SetDuty(m1, dutyCycle);
			SetDuty(m2, 100 - dutyCycle);
			Serial.print("dutyCycle m1: ");
			Serial.println(dutyCycle, DEC);
			Serial.print("dutyCycle m2: ");
			Serial.println((100 - dutyCycle), DEC);
			delay(80);
		}
		delay(1000);
		// rpm down
		for (int dutyCycle = 100; dutyCycle >= 0; dutyCycle--) {
			SetDuty(m1, dutyCycle);
			SetDuty(m2, 100 - dutyCycle);
			Serial.print("dutyCycle m1: ");
			Serial.println(dutyCycle, DEC);
			Serial.print("dutyCycle m2: ");
			Serial.println((100 - dutyCycle), DEC);
			delay(80);
		}
		delay(1000);
	}
}
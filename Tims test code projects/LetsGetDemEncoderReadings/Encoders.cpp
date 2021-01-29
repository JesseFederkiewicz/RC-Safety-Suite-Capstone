/*
 Name:		TimerCode.c
 Created:	1/28/2021
 Author:	Tim/Jesse
*/
#include "Encoders.h"
#include "driver/mcpwm.h"
#include "esp32-hal.h"
#include "driver/gpio.h"
#include "HardwareSerial.h"
#include "driver/timer.h"
#include "driver/pcnt.h"
#include "arduino.h"

volatile bool intFlag = false; // flag for use in main for actual code to run every interrupt interval

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // used for syncing main and isr, ignore this red squiggle, still works

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
	mcpwm_capture_on_edge_t edgeCapture; // which edge detection to use, pos/neg
};

// bundle up each struct into a nice group for each motor
typedef struct Motor_Settings {
	PWM_Settings pwm;
	Encoder_Settings encoder;
};

// this setup will need to be run for each motor
void PWMSetup(Motor_Settings m, mcpwm_config_t* unitConf)
{
	// attach pwm signal ouptput to gpio pin
	mcpwm_gpio_init(m.pwm.unit, m.pwm.signal, m.pwm.pin);

	// init the pwm unit, not actually needed for each motor, only for each unit... but oh well
	mcpwm_init(m.pwm.unit, m.pwm.timer, unitConf);
}

void PCNTSetup(Motor_Settings m1, Motor_Settings m2)
{
	pcnt_config_t m1Enc;
	m1Enc.pulse_gpio_num = m1.encoder.pin;   // Encoder input pin (GPIO32)
	m1Enc.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	m1Enc.unit = PCNT_UNIT_0;                // motor 1 encoder readings using pcnt unit 0
	m1Enc.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	m1Enc.pos_mode = PCNT_COUNT_INC;         // Count Only On Rising-Edges
	m1Enc.neg_mode = PCNT_COUNT_DIS;	     // Discard Falling-Edge
	m1Enc.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m1Enc.hctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m1Enc.counter_h_lim = INT16_MAX;
	m1Enc.counter_l_lim = INT16_MIN;

	pcnt_config_t m2Enc;
	m2Enc.pulse_gpio_num = m2.encoder.pin;   // Encoder input pin (GPIO32)
	m2Enc.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	m2Enc.unit = PCNT_UNIT_1;				 // motor 2 encoder readings using pcnt unit 1
	m2Enc.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	m2Enc.pos_mode = PCNT_COUNT_INC;         // Count Only On Rising-Edges
	m2Enc.neg_mode = PCNT_COUNT_DIS;	     // Discard Falling-Edge
	m2Enc.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m2Enc.hctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m2Enc.counter_h_lim = INT16_MAX;
	m2Enc.counter_l_lim = INT16_MIN;

	pcnt_unit_config(&m1Enc);
	pcnt_unit_config(&m2Enc);

	// set pulse pcnt filters to filter unwanted noise
	pcnt_set_filter_value(PCNT_UNIT_0, 250);
	pcnt_filter_enable(PCNT_UNIT_0);
	pcnt_set_filter_value(PCNT_UNIT_1, 250);
	pcnt_filter_enable(PCNT_UNIT_1);

	gpio_set_direction(GPIO_NUM_34, GPIO_MODE_INPUT);
	gpio_set_direction(GPIO_NUM_35, GPIO_MODE_INPUT);

	// enable pullup resistors
	gpio_pullup_en(GPIO_NUM_34); 
	gpio_pullup_en(GPIO_NUM_35); 

	// enable pulldown resistors
	gpio_pulldown_en(GPIO_NUM_34);
	gpio_pulldown_en(GPIO_NUM_35);
}

// timer ISR, trips a flag to use in main
void IRAM_ATTR TimerInt()
{
	portENTER_CRITICAL_ISR(&timerMux);
	intFlag = true;
	portEXIT_CRITICAL_ISR(&timerMux);
}

void Main()
{
	// motor one configs
	Motor_Settings m1;
	m1.pwm.unit = MCPWM_UNIT_0;               // m1 using pwm unit 0
	m1.pwm.timer = MCPWM_TIMER_0;             // m1 using unit 0 timer 0
	m1.pwm.opOut = MCPWM_OPR_A;               // m1 on operator 0 output A
	m1.pwm.signal = MCPWM0A;                  // sort of the same as above, needed for gpio_init
	m1.pwm.pin = 22;                          // pwm for m1 on pin 22
	m1.encoder.pin = 34;                      // encoder input on pin 34
	m1.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges

	// motor two configs
	Motor_Settings m2;
	m2.pwm.unit = m1.pwm.unit;                // m2 shares unit with m1
	m2.pwm.timer = m1.pwm.timer;              // m2 shares timer with m1
	m2.pwm.opOut = MCPWM_OPR_B;               // m2 on operator 0 output B
	m2.pwm.signal = MCPWM0B;                  // needed for gpio_init
	m2.pwm.pin = 23;		                  // pwm for m1 on pin 23
	m2.encoder.pin = 35;                      // encoder input on pin 35
	m2.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges

	// config for pwm unit 0 timer 0 
	mcpwm_config_t pwmconf;
	pwmconf.frequency = 10000;				 // timer freq 10k
	pwmconf.duty_mode = MCPWM_DUTY_MODE_0;   // positive duty cycle
	pwmconf.counter_mode = MCPWM_UP_COUNTER; // timer up counter
	pwmconf.cmpr_a = 0;						 // initial duty of 0
	pwmconf.cmpr_b = 0;

	// run setup for each motor
	PWMSetup(m1, &pwmconf);
	PWMSetup(m2, &pwmconf);

	mcpwm_set_duty(m1.pwm.unit, m1.pwm.timer, m1.pwm.opOut, 100);
	mcpwm_set_duty(m2.pwm.unit, m2.pwm.timer, m2.pwm.opOut, 50);

	PCNTSetup(m1, m2);

	Serial.begin(115200);

	hw_timer_t* timer = NULL;

	/*
		set up initial timer configs
		First Param:	What timer to use (0)
		Second Param:	Prescale (80) allows 1Mhz clock
		Third Param:	Count mode (true) count up mode.
		Return:			hw_timer_t (timer)
	 */
	timer = timerBegin(0, 80, true);

	/*
		Attach Timer to ISR
		First Param:	Which timer, hw_timer_t (timer)
		Second Param:	Callback method for ISR (TimerInt)
		Third Param:	Edge: (true)
	*/
	timerAttachInterrupt(timer, &TimerInt, true);

	/*
		How often to trigger alarm
		First Param:	Which timer, hw_timer_t (timer)
		Second Param:	Timer counter value when interupt triggers, currently triggering every 1s (1000000) (1Mhz / 1000000)
		Third Param:	Reset interrupt flag and timer counter (true)
	*/
	timerAlarmWrite(timer, 1000000, true);

	/*
		Enable alarm (interrupt)
		First Param:	Which timer, hw_timer_t (timer)
	*/
	timerAlarmEnable(timer);

	int16_t enc_count1;
	int16_t enc_count2;

	for (;;)
	{
		// currently reading values every second with interrupt timer
		if (intFlag)
		{
			/*
				RPM = (encoder count)/(211.2 counts per revolution)*(60 rpm)*(4 (only 1 rising edge reading)); 
				motor specs: https://www.pololu.com/product/4861
			*/
			pcnt_get_counter_value(PCNT_UNIT_0, &enc_count1);
			pcnt_get_counter_value(PCNT_UNIT_1, &enc_count2);

			Serial.print("RPM1:");

			Serial.println((float)enc_count1 / 211.2 * 240.0); 

			Serial.print("RPM2:");

			Serial.println((float)enc_count2 / 211.2 * 240.0);

			pcnt_counter_clear(PCNT_UNIT_0);
			pcnt_counter_clear(PCNT_UNIT_1);

			portENTER_CRITICAL(&timerMux);
			intFlag = false;
			portEXIT_CRITICAL(&timerMux);
		}
	}
}
/*
 Name:		TimerCode.c
 Created:	1/28/2021
 Author:	Tim/Jesse
*/
#include "Motor_Encoder.h"
#include "driver/mcpwm.h"
#include "esp32-hal.h"
#include "driver/gpio.h"
#include "HardwareSerial.h"
#include "driver/timer.h"
#include "driver/pcnt.h"
#include "arduino.h"

// interrupt timer stuff:

volatile bool intFlag = false; // flag for use in main for actual code to run every interrupt interval
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // used for syncing main and isr, ignore this red squiggle, still works

// timer ISR, trips a flag to use in main
void IRAM_ATTR TimerInt()
{
	portENTER_CRITICAL_ISR(&timerMux);
	intFlag = true;
	portEXIT_CRITICAL_ISR(&timerMux);
}

// this setup will need to be run for each motor
void PWMSetup(Motor_Settings m, mcpwm_config_t* unitConf)
{
	// attach pwm signal ouptput to gpio pin
	mcpwm_gpio_init(m.pwm.unit, m.pwm.signal, m.pwm.pin);

	// init the pwm unit, not actually needed for each motor, only for each unit... but oh well
	mcpwm_init(m.pwm.unit, m.pwm.timer, unitConf);
}

pcnt_config_t m1Enc;
pcnt_config_t m2Enc;
pcnt_config_t m3Enc;
pcnt_config_t m4Enc;
// run this once for all motors
void PCNTSetup(Motor_Settings m1, Motor_Settings m2, Motor_Settings m3, Motor_Settings m4)
{
	//pcnt_config_t m1Enc;
	m1Enc.pulse_gpio_num = m1.encoder.pin;   // Encoder input pin (GPIO34)
	m1Enc.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	m1Enc.unit = PCNT_UNIT_0;                // motor 1 encoder readings using pcnt unit 0
	m1Enc.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	m1Enc.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	m1Enc.neg_mode = PCNT_COUNT_INC;	     // 
	m1Enc.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m1Enc.hctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m1Enc.counter_h_lim = INT16_MAX;
	m1Enc.counter_l_lim = INT16_MIN;

	//pcnt_config_t m2Enc;
	m2Enc.pulse_gpio_num = m2.encoder.pin;   // Encoder input pin (GPIO35)
	m2Enc.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	m2Enc.unit = PCNT_UNIT_1;				 // motor 2 encoder readings using pcnt unit 1
	m2Enc.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	m2Enc.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	m2Enc.neg_mode = PCNT_COUNT_INC;	     // 
	m2Enc.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m2Enc.hctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m2Enc.counter_h_lim = INT16_MAX;
	m2Enc.counter_l_lim = INT16_MIN;

	//pcnt_config_t m3Enc;
	m3Enc.pulse_gpio_num = m3.encoder.pin;   // Encoder input pin (GPIO12)
	m3Enc.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	m3Enc.unit = PCNT_UNIT_2;                // motor 1 encoder readings using pcnt unit 0
	m3Enc.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	m3Enc.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	m3Enc.neg_mode = PCNT_COUNT_INC;	     // 
	m3Enc.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m3Enc.hctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m3Enc.counter_h_lim = INT16_MAX;
	m3Enc.counter_l_lim = INT16_MIN;

	//pcnt_config_t m4Enc;
	m4Enc.pulse_gpio_num = m4.encoder.pin;   // Encoder input pin (GPIO14)
	m4Enc.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	m4Enc.unit = PCNT_UNIT_3;				 // motor 2 encoder readings using pcnt unit 1
	m4Enc.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	m4Enc.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	m4Enc.neg_mode = PCNT_COUNT_INC;	     // 
	m4Enc.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m4Enc.hctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m4Enc.counter_h_lim = INT16_MAX;
	m4Enc.counter_l_lim = INT16_MIN;

	pcnt_unit_config(&m1Enc);
	pcnt_unit_config(&m2Enc);
	pcnt_unit_config(&m3Enc);
	pcnt_unit_config(&m4Enc);

	// set pulse pcnt filters to filter unwanted noise
	pcnt_set_filter_value(PCNT_UNIT_0, 250);
	pcnt_filter_enable(PCNT_UNIT_0);
	pcnt_set_filter_value(PCNT_UNIT_1, 250);
	pcnt_filter_enable(PCNT_UNIT_1);
	pcnt_set_filter_value(PCNT_UNIT_2, 250);
	pcnt_filter_enable(PCNT_UNIT_2);
	pcnt_set_filter_value(PCNT_UNIT_3, 250);
	pcnt_filter_enable(PCNT_UNIT_3);

	gpio_set_direction(m1.encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(m2.encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(m3.encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(m4.encoder.gpioNum, GPIO_MODE_INPUT);

	// enable pullup resistors
	gpio_pullup_en(m1.encoder.gpioNum);
	gpio_pullup_en(m2.encoder.gpioNum);
	gpio_pullup_en(m3.encoder.gpioNum);
	gpio_pullup_en(m4.encoder.gpioNum);

	// enable pulldown resistors
	gpio_pulldown_en(m1.encoder.gpioNum);
	gpio_pulldown_en(m2.encoder.gpioNum);
	gpio_pulldown_en(m3.encoder.gpioNum);
	gpio_pulldown_en(m4.encoder.gpioNum);
}

const int _timerPrescale = 80;
const int _timerClk = 80000000 / _timerPrescale;
const int _intTriggerPeriod_ms = 50;

float CalcRPM(uint encoderVal) {
	/*
		RPM = (encoder count)/(211.2 counts per revolution)*(60 rpm)*(2 [only using 1 encoder signal])*(_timerClk/intTriggerPeriodUs);
		motor specs: https://www.pololu.com/product/4861
	*/
	return encoderVal / 211.2 * 120.0 * (float)(_timerClk / (_intTriggerPeriod_ms * 1000.0));
}

void MotorAndEncoderInits()
{
	// Motor setups:

	// motor one configs
	Motor_Settings frontLeftMotor;
	frontLeftMotor.encoder.pin = 34;                     // encoder input on pin 34
	frontLeftMotor.encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	frontLeftMotor.encoder.gpioNum = GPIO_NUM_34;        // gpio num should match pin num

	// motor two configs
	Motor_Settings frontRightMotor;
	frontRightMotor.encoder.pin = 35;                     // encoder input on pin 35
	frontRightMotor.encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	frontRightMotor.encoder.gpioNum = GPIO_NUM_35;        // gpio num should match pin num

	// motor three configs
	Motor_Settings backLeftMotor;
	backLeftMotor.encoder.pin = 27;                     // encoder input on pin 12
	backLeftMotor.encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	backLeftMotor.encoder.gpioNum = GPIO_NUM_27;        // gpio num should match pin num

	// motor four configs
	Motor_Settings backRightMotor;
	backRightMotor.encoder.pin = 14;                     // encoder input on pin 14
	backRightMotor.encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	backRightMotor.encoder.gpioNum = GPIO_NUM_14;        // gpio num should match pin num

	//// config for pwm unit 0 timer 0 
	//mcpwm_config_t pwmconf;
	//pwmconf.frequency = 10000;				 // timer freq 10k
	//pwmconf.duty_mode = MCPWM_DUTY_MODE_0;   // positive duty cycle
	//pwmconf.counter_mode = MCPWM_UP_COUNTER; // timer up counter
	//pwmconf.cmpr_a = 0;						 // initial duty of 0
	//pwmconf.cmpr_b = 0;

	//// run setup for each motor
	//PWMSetup(frontLeftMotor, &pwmconf);
	//PWMSetup(frontRightMotor, &pwmconf);
	//PWMSetup(backLeftMotor, &pwmconf);
	//PWMSetup(backRightMotor, &pwmconf);

	// Encoder Setups:

	PCNTSetup(frontLeftMotor, frontRightMotor, backLeftMotor, backRightMotor);

	// Direction controls:

	// setup gpio pin 21 for rightDirection control
	gpio_num_t rightDirectionControl = GPIO_NUM_21;
	gpio_config_t dirConfigRight;
	dirConfigRight.intr_type = GPIO_INTR_DISABLE;
	dirConfigRight.mode = GPIO_MODE_OUTPUT;
	dirConfigRight.pin_bit_mask = 0b1000000000000000000000; // bit #21 for pin 21
	dirConfigRight.pull_down_en = GPIO_PULLDOWN_ENABLE;
	dirConfigRight.pull_up_en = GPIO_PULLUP_ENABLE;

	// setup gpio pin 4 for leftDirection control
	gpio_num_t leftDirectionControl = GPIO_NUM_4;
	gpio_config_t dirConfigLeft;
	dirConfigLeft.intr_type = GPIO_INTR_DISABLE;
	dirConfigLeft.mode = GPIO_MODE_OUTPUT;
	dirConfigLeft.pin_bit_mask = 0b10000; // bit #4 for pin 4
	dirConfigLeft.pull_down_en = GPIO_PULLDOWN_ENABLE;
	dirConfigLeft.pull_up_en = GPIO_PULLUP_ENABLE;

	gpio_config(&dirConfigRight);
	gpio_config(&dirConfigLeft);

	// set direction Forward for now
	gpio_set_level(rightDirectionControl, 0);
	gpio_set_level(leftDirectionControl, 1);

	// Timer configs	

	hw_timer_t* timer = NULL;

	/*
		set up initial timer configs
		First Param:	What timer to use (0)
		Second Param:	Prescale (80) allows 1Mhz clock
		Third Param:	Count mode (true) count up mode.
		Return:			hw_timer_t (timer)
	 */
	timer = timerBegin(0, _timerPrescale, true);

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
	timerAlarmWrite(timer, _intTriggerPeriod_ms * 1000, true);

	/*
		Enable alarm (interrupt)
		First Param:	Which timer, hw_timer_t (timer)
	*/
	timerAlarmEnable(timer);

	//mcpwm_set_duty(frontLeftMotor.pwm.unit, frontLeftMotor.pwm.timer, frontLeftMotor.pwm.opOut, 100);
	//mcpwm_set_duty(frontRightMotor.pwm.unit, frontRightMotor.pwm.timer, frontRightMotor.pwm.opOut, 100);
	//mcpwm_set_duty(backLeftMotor.pwm.unit, backLeftMotor.pwm.timer, backLeftMotor.pwm.opOut, 100);
	//mcpwm_set_duty(backRightMotor.pwm.unit, backRightMotor.pwm.timer, backRightMotor.pwm.opOut, 100);

	//Serial.begin(115200);

	//int16_t fl_Enc;
	//int16_t fr_Enc;
	//int16_t bl_Enc;
	//int16_t br_Enc;

	//for (;;)
	//{
	//	// currently reading values every second with interrupt timer
	//	if (intFlag)
	//	{
	//		pcnt_get_counter_value(PCNT_UNIT_0, &fl_Enc);
	//		pcnt_get_counter_value(PCNT_UNIT_1, &fr_Enc);
	//		pcnt_get_counter_value(PCNT_UNIT_2, &bl_Enc);
	//		pcnt_get_counter_value(PCNT_UNIT_3, &br_Enc);

	//		Serial.println(fl_Enc);
	//		Serial.println(fr_Enc);
	//		Serial.println(bl_Enc);
	//		Serial.println(br_Enc);

	//		//Serial.print("FL enc:");
	//		//Serial.println(fl_Enc);
	//		//Serial.print("RPM1:");
	//		//Serial.println(CalcRPM(fl_Enc));
	//		//Serial.println();

	//		//Serial.print("FR enc:");
	//		//Serial.println(fr_Enc);
	//		//Serial.print("RPM2:");
	//		//Serial.println(CalcRPM(fr_Enc));
	//		//Serial.println();

	//		//Serial.print("BL enc:");
	//		//Serial.println(bl_Enc);
	//		//Serial.print("RPM3:");
	//		//Serial.println(CalcRPM(bl_Enc));
	//		//Serial.println();

	//		//Serial.print("BR enc:");
	//		//Serial.println(br_Enc);
	//		//Serial.print("RPM4:");
	//		//Serial.println(CalcRPM(br_Enc));
	//		//Serial.println();

	//		pcnt_counter_clear(PCNT_UNIT_0);
	//		pcnt_counter_clear(PCNT_UNIT_1);
	//		pcnt_counter_clear(PCNT_UNIT_2);
	//		pcnt_counter_clear(PCNT_UNIT_3);

	//		portENTER_CRITICAL(&timerMux);
	//		intFlag = false;
	//		portEXIT_CRITICAL(&timerMux);
	//	}
	//}
}

//// calculates motor rpm based on encoder readings
//// RPM = (encoder count)/(211.2 counts per revolution)*(60 rpm)*(2 [only using 1 encoder signal])*(_timerClk/intTriggerPeriodUs);	
//// motor specs: https://www.pololu.com/product/4861
//float CalcRPM(uint encoderVal)
//{
//	const int timerClk = 80000000 / _timerPrescale;
//	return encoderVal / 211.2 * 120.0 * (float)(timerClk / (_intTriggerPeriod_ms * 1000.0));
//}
//
// reads all encoder values from the pulse counter modules, clears the counters
// gets RPMs from the encoder values and returns them
RPMS GetRPMS()
{
	int16_t fl_Enc;
	int16_t fr_Enc;
	int16_t bl_Enc;
	int16_t br_Enc;

	RPMS rpmOutput;

	pcnt_get_counter_value(m1Enc.unit, &fl_Enc);
	pcnt_get_counter_value(m2Enc.unit, &fr_Enc);
	pcnt_get_counter_value(m3Enc.unit, &bl_Enc);
	pcnt_get_counter_value(m4Enc.unit, &br_Enc);

	rpmOutput.FL_RPM = CalcRPM(fl_Enc);
	rpmOutput.FR_RPM = CalcRPM(fr_Enc);
	rpmOutput.BL_RPM = CalcRPM(bl_Enc);
	rpmOutput.BR_RPM = CalcRPM(br_Enc);

	Serial.println(fl_Enc);
	Serial.println(fr_Enc);
	Serial.println(bl_Enc);
	Serial.println(br_Enc);

	pcnt_counter_clear(m1Enc.unit);
	pcnt_counter_clear(m2Enc.unit);
	pcnt_counter_clear(m3Enc.unit);
	pcnt_counter_clear(m4Enc.unit);

	return rpmOutput;
}
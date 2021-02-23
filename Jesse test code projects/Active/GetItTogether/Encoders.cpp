// 
// File: Encoders.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// 

#include "Encoders.h"

// timer interrupt globals, used in init, and RPM calcs
const int _timerPrescale = 80;       // prescale for interrupt timer clock settings
const int _intTriggerPeriod_ms = 50; // the period in which we will trigger timer interrupts in ms

Encoder_Settings FL_Encoder;
Encoder_Settings FR_Encoder;
Encoder_Settings BL_Encoder;
Encoder_Settings BR_Encoder;

///likely issue here with encoders

void InitEncoders()
{
	FL_Encoder.pin = 34;                     // Front Left encoder input on pin 34
	FL_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	FL_Encoder.gpioNum = GPIO_NUM_34;        // gpio num should match pin num
	FL_Encoder.pcntUnit = PCNT_UNIT_0;       // motor 1 encoder readings using pcnt unit 0

	FR_Encoder.pin = 35;                     // Front Right encoder input on pin 35
	FR_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	FR_Encoder.gpioNum = GPIO_NUM_35;        // gpio num should match pin num
	FR_Encoder.pcntUnit = PCNT_UNIT_1;       // motor 2 encoder readings using pcnt unit 1

	BL_Encoder.pin = 27;                     // Back Left encoder input on pin 12
	BL_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	BL_Encoder.gpioNum = GPIO_NUM_27;        // gpio num should match pin num
	BL_Encoder.pcntUnit = PCNT_UNIT_2;       // motor 3 encoder readings using pcnt unit 2

	BR_Encoder.pin = 14;                     // Back Right encoder input on pin 14
	BR_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	BR_Encoder.gpioNum = GPIO_NUM_14;        // gpio num should match pin num
	BR_Encoder.pcntUnit = PCNT_UNIT_3;       // motor 4 encoder readings using pcnt unit 3

	pcnt_config_t FL_PCNT;
	FL_PCNT.pulse_gpio_num = FL_Encoder.pin;   // Encoder input pin (GPIO34)
	FL_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	FL_PCNT.unit = FL_Encoder.pcntUnit;        // motor 1 encoder readings using pcnt unit 0        
	FL_PCNT.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	FL_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	FL_PCNT.neg_mode = PCNT_COUNT_INC;	       // 
	FL_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	FL_PCNT.hctrl_mode = PCNT_MODE_KEEP;       //
	FL_PCNT.counter_h_lim = INT16_MAX;
	FL_PCNT.counter_l_lim = INT16_MIN;

	pcnt_config_t FR_PCNT;
	FR_PCNT.pulse_gpio_num = FR_Encoder.pin;   // Encoder input pin (GPIO35)
	FR_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	FR_PCNT.unit = FR_Encoder.pcntUnit;		   // motor 2 encoder readings using pcnt unit 1
	FR_PCNT.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	FR_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	FR_PCNT.neg_mode = PCNT_COUNT_INC;	       // 
	FR_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	FR_PCNT.hctrl_mode = PCNT_MODE_KEEP;       //
	FR_PCNT.counter_h_lim = INT16_MAX;
	FR_PCNT.counter_l_lim = INT16_MIN;

	pcnt_config_t BL_PCNT;
	BL_PCNT.pulse_gpio_num = BL_Encoder.pin;   // Encoder input pin (GPIO12)
	BL_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	BL_PCNT.unit = BL_Encoder.pcntUnit;        // motor 3 encoder readings using pcnt unit 2
	BL_PCNT.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	BL_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	BL_PCNT.neg_mode = PCNT_COUNT_INC;	       // 
	BL_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	BL_PCNT.hctrl_mode = PCNT_MODE_KEEP;       //
	BL_PCNT.counter_h_lim = INT16_MAX;
	BL_PCNT.counter_l_lim = INT16_MIN;

	pcnt_config_t BR_PCNT;
	BR_PCNT.pulse_gpio_num = BR_Encoder.pin;   // Encoder input pin (GPIO14)
	BR_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	BR_PCNT.unit = BR_Encoder.pcntUnit;		   // motor 4 encoder readings using pcnt unit 3
	BR_PCNT.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	BR_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	BR_PCNT.neg_mode = PCNT_COUNT_INC;	       // 
	BR_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	BR_PCNT.hctrl_mode = PCNT_MODE_KEEP;       //
	BR_PCNT.counter_h_lim = INT16_MAX;
	BR_PCNT.counter_l_lim = INT16_MIN;

	pcnt_unit_config(&FL_PCNT);
	pcnt_unit_config(&FR_PCNT);
	pcnt_unit_config(&BL_PCNT);
	pcnt_unit_config(&BR_PCNT);

	// set pulse pcnt filters to filter unwanted noise
	pcnt_set_filter_value(FL_Encoder.pcntUnit, 250);
	pcnt_filter_enable(FL_Encoder.pcntUnit);
	pcnt_set_filter_value(FR_Encoder.pcntUnit, 250);
	pcnt_filter_enable(FR_Encoder.pcntUnit);
	pcnt_set_filter_value(BL_Encoder.pcntUnit, 250);
	pcnt_filter_enable(BL_Encoder.pcntUnit);
	pcnt_set_filter_value(BR_Encoder.pcntUnit, 250);
	pcnt_filter_enable(BR_Encoder.pcntUnit);

	gpio_set_direction(FL_Encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(FR_Encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(BL_Encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(BR_Encoder.gpioNum, GPIO_MODE_INPUT);

	// enable pullup resistors
	gpio_pullup_en(FL_Encoder.gpioNum);
	gpio_pullup_en(FR_Encoder.gpioNum);
	gpio_pullup_en(BL_Encoder.gpioNum);
	gpio_pullup_en(BR_Encoder.gpioNum);

	// enable pulldown resistors
	gpio_pulldown_en(FL_Encoder.gpioNum);
	gpio_pulldown_en(FR_Encoder.gpioNum);
	gpio_pulldown_en(BL_Encoder.gpioNum);
	gpio_pulldown_en(BR_Encoder.gpioNum);
}

// setup function for timer interrupt
// takes a timer ISR function as an argument
void TimerInterruptInit(void (*intFunc) (void))
{
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
		Second Param:	Callback method for ISR (intFunc param)
		Third Param:	Edge: (true)
	*/
	timerAttachInterrupt(timer, intFunc, true);

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
}

// calculates motor rpm based on encoder readings
// RPM = (encoder count)/(211.2 counts per revolution)*(60 rpm)*(2 [only using 1 encoder signal])*(_timerClk/intTriggerPeriodUs);	
// motor specs: https://www.pololu.com/product/4861
float CalcRMP(uint encoderVal)
{
	const int timerClk = 80000000 / _timerPrescale;
	return encoderVal / 211.2 * 120.0 * (float)(timerClk / (_intTriggerPeriod_ms * 1000.0));
}

// reads all encoder values from the pulse counter modules, clears the counters
// gets RPMs from the encoder values and returns them
RPMS GetRPMS()
{
	int16_t fl_Enc;
	int16_t fr_Enc;
	int16_t bl_Enc;
	int16_t br_Enc;

	RPMS rpmOutput;

	pcnt_get_counter_value(FL_Encoder.pcntUnit, &fl_Enc);
	pcnt_get_counter_value(FR_Encoder.pcntUnit, &fr_Enc);
	pcnt_get_counter_value(BL_Encoder.pcntUnit, &bl_Enc);
	pcnt_get_counter_value(BR_Encoder.pcntUnit, &br_Enc);

	rpmOutput.FL_RPM = CalcRMP(fl_Enc);
	rpmOutput.FR_RPM = CalcRMP(fr_Enc);
	rpmOutput.BL_RPM = CalcRMP(bl_Enc);
	rpmOutput.BR_RPM = CalcRMP(br_Enc);

	//Serial.println(fl_Enc);
	//Serial.println(fr_Enc);
	//Serial.println(bl_Enc);
	//Serial.println(br_Enc);

	pcnt_counter_clear(FL_Encoder.pcntUnit);
	pcnt_counter_clear(FR_Encoder.pcntUnit);
	pcnt_counter_clear(BL_Encoder.pcntUnit);
	pcnt_counter_clear(BR_Encoder.pcntUnit);

	return rpmOutput;
}
// 
// File: Encoders.cpp
// Authors: Tim Hachey/Jesse Federkiewicz
// Description: contains functions for initializing the pulse counters used for reading encoders,
//				and calculating rpms for all wheels based on the pulse counter values
// 

#include "Encoders.h"

// timer interrupt globals, used in init, and RPM calcs
const int _timerPrescale = 80;       // prescale for interrupt timer clock settings
const int _intTriggerPeriod_ms = 50; // the period in which we will trigger timer interrupts in ms

// settings configs for all 8 PCNT units used (2 per encoder per motor)
Encoder_Settings FL_Encoder;
Encoder_Settings FL_Encoder2;
Encoder_Settings FR_Encoder;
Encoder_Settings FR_Encoder2;
Encoder_Settings BL_Encoder;
Encoder_Settings BL_Encoder2;
Encoder_Settings BR_Encoder;
Encoder_Settings BR_Encoder2;

void InitEncoders(void (*LFintISR) (void), void (*RFintISR) (void), void (*RRintISR) (void), void (*LRintISR) (void))
{
	FL_Encoder.pin = 34;                     // Front Left encoder input on pin 34
	FL_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	FL_Encoder.gpioNum = GPIO_NUM_34;        // gpio num should match pin num
	FL_Encoder.pcntUnit = PCNT_UNIT_0;       // motor 1 encoder readings using pcnt unit 0

	FL_Encoder2.pin = 32;                     // Front Left encoder input on pin 32
	FL_Encoder2.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	FL_Encoder2.gpioNum = GPIO_NUM_32;        // gpio num should match pin num
	FL_Encoder2.pcntUnit = PCNT_UNIT_4;       // motor 1 encoder readings using pcnt unit 4

	FR_Encoder.pin = 35;                     // Front Right encoder input on pin 35
	FR_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	FR_Encoder.gpioNum = GPIO_NUM_35;        // gpio num should match pin num
	FR_Encoder.pcntUnit = PCNT_UNIT_1;       // motor 2 encoder readings using pcnt unit 1

	FR_Encoder2.pin = 33;                     // Front Right encoder input on pin 33
	FR_Encoder2.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	FR_Encoder2.gpioNum = GPIO_NUM_33;        // gpio num should match pin num
	FR_Encoder2.pcntUnit = PCNT_UNIT_5;       // motor 2 encoder readings using pcnt unit 5

	BL_Encoder.pin = 27;                     // Back Left encoder input on pin 27
	BL_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	BL_Encoder.gpioNum = GPIO_NUM_27;        // gpio num should match pin num
	BL_Encoder.pcntUnit = PCNT_UNIT_2;       // motor 3 encoder readings using pcnt unit 2

	BL_Encoder2.pin = 26;                     // Back Left encoder input on pin 26
	BL_Encoder2.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	BL_Encoder2.gpioNum = GPIO_NUM_26;        // gpio num should match pin num
	BL_Encoder2.pcntUnit = PCNT_UNIT_6;       // motor 3 encoder readings using pcnt unit 6

	BR_Encoder.pin = 14;                     // Back Right encoder input on pin 14
	BR_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	BR_Encoder.gpioNum = GPIO_NUM_14;        // gpio num should match pin num
	BR_Encoder.pcntUnit = PCNT_UNIT_3;       // motor 4 encoder readings using pcnt unit 3

	BR_Encoder2.pin = 25;                     // Back Right encoder input on pin 25
	BR_Encoder2.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	BR_Encoder2.gpioNum = GPIO_NUM_25;        // gpio num should match pin num
	BR_Encoder2.pcntUnit = PCNT_UNIT_7;       // motor 4 encoder readings using pcnt unit 7

	pcnt_config_t FL_PCNT;
	FL_PCNT.pulse_gpio_num = FL_Encoder.pin;   // Encoder input pin
	FL_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	FL_PCNT.unit = FL_Encoder.pcntUnit;        // motor 1 encoder readings using pcnt unit 0        
	FL_PCNT.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	FL_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	FL_PCNT.neg_mode = PCNT_COUNT_INC;
	FL_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	FL_PCNT.hctrl_mode = PCNT_MODE_KEEP;
	FL_PCNT.counter_h_lim = INT16_MAX;
	FL_PCNT.counter_l_lim = INT16_MIN;

	pcnt_config_t FL_PCNT2;
	FL_PCNT2.pulse_gpio_num = FL_Encoder2.pin;  // Encoder input pin
	FL_PCNT2.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	FL_PCNT2.unit = FL_Encoder2.pcntUnit;       // motor 1 encoder readings using pcnt unit 0        
	FL_PCNT2.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	FL_PCNT2.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	FL_PCNT2.neg_mode = PCNT_COUNT_INC;
	FL_PCNT2.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	FL_PCNT2.hctrl_mode = PCNT_MODE_KEEP;
	FL_PCNT2.counter_h_lim = INT16_MAX;
	FL_PCNT2.counter_l_lim = INT16_MIN;

	pcnt_config_t FR_PCNT;
	FR_PCNT.pulse_gpio_num = FR_Encoder.pin;   // Encoder input pin 
	FR_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	FR_PCNT.unit = FR_Encoder.pcntUnit;		   // motor 2 encoder readings using pcnt unit 1
	FR_PCNT.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	FR_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	FR_PCNT.neg_mode = PCNT_COUNT_INC;
	FR_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	FR_PCNT.hctrl_mode = PCNT_MODE_KEEP;
	FR_PCNT.counter_h_lim = INT16_MAX;
	FR_PCNT.counter_l_lim = INT16_MIN;

	pcnt_config_t FR_PCNT2;
	FR_PCNT2.pulse_gpio_num = FR_Encoder2.pin;  // Encoder input pin
	FR_PCNT2.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	FR_PCNT2.unit = FR_Encoder2.pcntUnit;		// motor 2 encoder readings using pcnt unit 1
	FR_PCNT2.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	FR_PCNT2.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	FR_PCNT2.neg_mode = PCNT_COUNT_INC;
	FR_PCNT2.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	FR_PCNT2.hctrl_mode = PCNT_MODE_KEEP;
	FR_PCNT2.counter_h_lim = INT16_MAX;
	FR_PCNT2.counter_l_lim = INT16_MIN;

	pcnt_config_t BL_PCNT;
	BL_PCNT.pulse_gpio_num = BL_Encoder.pin;   // Encoder input pin 
	BL_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	BL_PCNT.unit = BL_Encoder.pcntUnit;        // motor 3 encoder readings using pcnt unit 2
	BL_PCNT.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	BL_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	BL_PCNT.neg_mode = PCNT_COUNT_INC;
	BL_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	BL_PCNT.hctrl_mode = PCNT_MODE_KEEP;
	BL_PCNT.counter_h_lim = INT16_MAX;
	BL_PCNT.counter_l_lim = INT16_MIN;

	pcnt_config_t BL_PCNT2;
	BL_PCNT2.pulse_gpio_num = BL_Encoder2.pin;  // Encoder input pin
	BL_PCNT2.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	BL_PCNT2.unit = BL_Encoder2.pcntUnit;       // motor 3 encoder readings using pcnt unit 2
	BL_PCNT2.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	BL_PCNT2.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	BL_PCNT2.neg_mode = PCNT_COUNT_INC;
	BL_PCNT2.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	BL_PCNT2.hctrl_mode = PCNT_MODE_KEEP;
	BL_PCNT2.counter_h_lim = INT16_MAX;
	BL_PCNT2.counter_l_lim = INT16_MIN;

	pcnt_config_t BR_PCNT;
	BR_PCNT.pulse_gpio_num = BR_Encoder.pin;   // Encoder input pin 
	BR_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	BR_PCNT.unit = BR_Encoder.pcntUnit;		   // motor 4 encoder readings using pcnt unit 3
	BR_PCNT.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	BR_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	BR_PCNT.neg_mode = PCNT_COUNT_INC;
	BR_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	BR_PCNT.hctrl_mode = PCNT_MODE_KEEP;
	BR_PCNT.counter_h_lim = INT16_MAX;
	BR_PCNT.counter_l_lim = INT16_MIN;

	pcnt_config_t BR_PCNT2;
	BR_PCNT2.pulse_gpio_num = BR_Encoder2.pin;  // Encoder input pin
	BR_PCNT2.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	BR_PCNT2.unit = BR_Encoder2.pcntUnit;		// motor 4 encoder readings using pcnt unit 3
	BR_PCNT2.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	BR_PCNT2.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	BR_PCNT2.neg_mode = PCNT_COUNT_INC;
	BR_PCNT2.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	BR_PCNT2.hctrl_mode = PCNT_MODE_KEEP;
	BR_PCNT2.counter_h_lim = INT16_MAX;
	BR_PCNT2.counter_l_lim = INT16_MIN;

	pcnt_unit_config(&FL_PCNT);
	pcnt_unit_config(&FL_PCNT2);
	pcnt_unit_config(&FR_PCNT);
	pcnt_unit_config(&FR_PCNT2);
	pcnt_unit_config(&BL_PCNT);
	pcnt_unit_config(&BL_PCNT2);
	pcnt_unit_config(&BR_PCNT);
	pcnt_unit_config(&BR_PCNT2);

	// set pulse pcnt filters to filter unwanted noise
	pcnt_set_filter_value(FL_Encoder.pcntUnit, 250);
	pcnt_filter_enable(FL_Encoder.pcntUnit);
	pcnt_set_filter_value(FL_Encoder2.pcntUnit, 250);
	pcnt_filter_enable(FL_Encoder2.pcntUnit);

	pcnt_set_filter_value(FR_Encoder.pcntUnit, 250);
	pcnt_filter_enable(FR_Encoder.pcntUnit);
	pcnt_set_filter_value(FR_Encoder2.pcntUnit, 250);
	pcnt_filter_enable(FR_Encoder2.pcntUnit);

	pcnt_set_filter_value(BL_Encoder.pcntUnit, 250);
	pcnt_filter_enable(BL_Encoder.pcntUnit);
	pcnt_set_filter_value(BL_Encoder2.pcntUnit, 250);
	pcnt_filter_enable(BL_Encoder2.pcntUnit);

	pcnt_set_filter_value(BR_Encoder.pcntUnit, 250);
	pcnt_filter_enable(BR_Encoder.pcntUnit);
	pcnt_set_filter_value(BR_Encoder2.pcntUnit, 250);
	pcnt_filter_enable(BR_Encoder2.pcntUnit);

	// set gpios as inputs
	gpio_set_direction(FL_Encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(FL_Encoder2.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(FR_Encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(FR_Encoder2.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(BL_Encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(BL_Encoder2.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(BR_Encoder.gpioNum, GPIO_MODE_INPUT);
	gpio_set_direction(BR_Encoder2.gpioNum, GPIO_MODE_INPUT);

	// enable pullup resistors
	gpio_pullup_en(FL_Encoder.gpioNum);
	gpio_pullup_en(FL_Encoder2.gpioNum);
	gpio_pullup_en(FR_Encoder.gpioNum);
	gpio_pullup_en(FR_Encoder2.gpioNum);
	gpio_pullup_en(BL_Encoder.gpioNum);
	gpio_pullup_en(BL_Encoder2.gpioNum);
	gpio_pullup_en(BR_Encoder.gpioNum);
	gpio_pullup_en(BR_Encoder2.gpioNum);

	// enable pulldown resistors
	gpio_pulldown_en(FL_Encoder.gpioNum);
	gpio_pulldown_en(FL_Encoder2.gpioNum);
	gpio_pulldown_en(FR_Encoder.gpioNum);
	gpio_pulldown_en(FR_Encoder2.gpioNum);
	gpio_pulldown_en(BL_Encoder.gpioNum);
	gpio_pulldown_en(BL_Encoder2.gpioNum);
	gpio_pulldown_en(BR_Encoder.gpioNum);
	gpio_pulldown_en(BR_Encoder2.gpioNum);

	// setup interrupt pins/ISRs for encoder XOR inputs
	attachInterrupt(GPIO_NUM_18, LFintISR, CHANGE);
	attachInterrupt(GPIO_NUM_19, RFintISR, CHANGE);
	attachInterrupt(GPIO_NUM_5, RRintISR, CHANGE);
	attachInterrupt(GPIO_NUM_13, LRintISR, CHANGE);
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
// RPM = (encoder count)/(211.2 counts per revolution)*(60 rpm)*(_timerClk/intTriggerPeriodUs);	
// motor specs: https://www.pololu.com/product/4861
float CalcRMP(uint encoderVal)
{
	const int _timerClk = 80000000 / _timerPrescale;	
	return encoderVal / 211.2 * 60.0 * (float)(_timerClk / (_intTriggerPeriod_ms * 1000.0));
}

// reads all encoder values from the pulse counter modules, clears the counters
// gets RPMs from the encoder values and returns them
void GetRPMS(RPMS* rpms)
{
	int16_t fl_Enc;
	int16_t fr_Enc;
	int16_t bl_Enc;
	int16_t br_Enc;
	int16_t fl_Enc2;
	int16_t fr_Enc2;
	int16_t bl_Enc2;
	int16_t br_Enc2;

	pcnt_get_counter_value(FL_Encoder.pcntUnit, &fl_Enc);
	pcnt_get_counter_value(FR_Encoder.pcntUnit, &fr_Enc);
	pcnt_get_counter_value(BL_Encoder.pcntUnit, &bl_Enc);
	pcnt_get_counter_value(BR_Encoder.pcntUnit, &br_Enc);	
	pcnt_get_counter_value(FL_Encoder2.pcntUnit, &fl_Enc2);
	pcnt_get_counter_value(FR_Encoder2.pcntUnit, &fr_Enc2);
	pcnt_get_counter_value(BL_Encoder2.pcntUnit, &bl_Enc2);
	pcnt_get_counter_value(BR_Encoder2.pcntUnit, &br_Enc2);

	// get rpms based on 2 encoders per wheel
	rpms->FL_RPM = CalcRMP(fl_Enc + fl_Enc2);
	rpms->FR_RPM = CalcRMP(fr_Enc + fr_Enc2);
	rpms->BL_RPM = CalcRMP(bl_Enc + bl_Enc2);
	rpms->BR_RPM = CalcRMP(br_Enc + br_Enc2);

	// set wheel directions to stopped on 0 RPM
	if (rpms->FL_RPM < 1) 
		rpms->FL_Wheel_movement = stopped;
	
	if (rpms->FR_RPM < 1) 
		rpms->FR_Wheel_movement = stopped;
	
	if (rpms->BL_RPM < 1) 
		rpms->BL_Wheel_movement = stopped;
	
	if (rpms->BR_RPM < 1) 
		rpms->BR_Wheel_movement = stopped;	

	// clear counts after read
	pcnt_counter_clear(FL_Encoder.pcntUnit);
	pcnt_counter_clear(FL_Encoder2.pcntUnit);
	pcnt_counter_clear(FR_Encoder.pcntUnit);
	pcnt_counter_clear(FR_Encoder2.pcntUnit);
	pcnt_counter_clear(BL_Encoder.pcntUnit);
	pcnt_counter_clear(BL_Encoder2.pcntUnit);
	pcnt_counter_clear(BR_Encoder.pcntUnit);
	pcnt_counter_clear(BR_Encoder2.pcntUnit);
}
// 
// 
// 

#include "GroundSpeed.h"

Encoder_Settings groundSpeed;

//Encoder_Settings BL_Encoder;

void GroundSpeedSensorInit()
{
	//Encoder_Settings groundSpeed;
	groundSpeed.pin = 13;                     // Front Left encoder input on pin 34
	groundSpeed.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	groundSpeed.gpioNum = GPIO_NUM_13;        // gpio num should match pin num
	groundSpeed.pcntUnit = PCNT_UNIT_4;       // motor 1 encoder readings using pcnt unit 0

	pcnt_config_t groundSpeed_PCNT;
	groundSpeed_PCNT.pulse_gpio_num = groundSpeed.pin;   // Encoder input pin (GPIO34)
	groundSpeed_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	groundSpeed_PCNT.unit = groundSpeed.pcntUnit;        // motor 1 encoder readings using pcnt unit 0        
	groundSpeed_PCNT.channel = PCNT_CHANNEL_0;          // using unit 4 channel 0
	groundSpeed_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	groundSpeed_PCNT.neg_mode = PCNT_COUNT_DIS;	       // 
	groundSpeed_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	groundSpeed_PCNT.hctrl_mode = PCNT_MODE_KEEP;       //
	groundSpeed_PCNT.counter_h_lim = INT16_MAX;
	groundSpeed_PCNT.counter_l_lim = INT16_MIN;

	pcnt_unit_config(&groundSpeed_PCNT);

	// set pulse pcnt filters to filter unwanted noise
	pcnt_set_filter_value(groundSpeed.pcntUnit, 250);
	pcnt_filter_enable(groundSpeed.pcntUnit);

	gpio_set_direction(groundSpeed.gpioNum, GPIO_MODE_INPUT);

	//// enable pullup resistors
	//gpio_pullup_en(groundSpeed.gpioNum);

	// enable pulldown resistors
	gpio_pulldown_en(groundSpeed.gpioNum);

	//BL_Encoder.pin = 27;                     // Back Left encoder input on pin 12
	//BL_Encoder.edgeCapture = MCPWM_POS_EDGE; // capture positive edges
	//BL_Encoder.gpioNum = GPIO_NUM_27;        // gpio num should match pin num
	//BL_Encoder.pcntUnit = PCNT_UNIT_2;       // motor 3 encoder readings using pcnt unit 2

	//pcnt_config_t BL_PCNT;
	//BL_PCNT.pulse_gpio_num = BL_Encoder.pin;   // Encoder input pin (GPIO12)
	//BL_PCNT.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	//BL_PCNT.unit = BL_Encoder.pcntUnit;        // motor 3 encoder readings using pcnt unit 2
	//BL_PCNT.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	//BL_PCNT.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	//BL_PCNT.neg_mode = PCNT_COUNT_INC;	       // 
	//BL_PCNT.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	//BL_PCNT.hctrl_mode = PCNT_MODE_KEEP;       //
	//BL_PCNT.counter_h_lim = INT16_MAX;
	//BL_PCNT.counter_l_lim = INT16_MIN;

	//pcnt_unit_config(&BL_PCNT);

	//pcnt_set_filter_value(BL_Encoder.pcntUnit, 250);
	//pcnt_filter_enable(BL_Encoder.pcntUnit);

	//gpio_set_direction(BL_Encoder.gpioNum, GPIO_MODE_INPUT);

	//gpio_pulldown_en(BL_Encoder.gpioNum);
}

int16_t GetGroundSpeedCounterVal()
{

	//int16_t bl_Enc = -1;

	//pcnt_get_counter_value(BL_Encoder.pcntUnit, &bl_Enc);

	//pcnt_counter_clear(BL_Encoder.pcntUnit);

	//return bl_Enc;
	int16_t val = -1;
	pcnt_get_counter_value(groundSpeed.pcntUnit, &val);
	pcnt_counter_clear(groundSpeed.pcntUnit);
	return val;
}


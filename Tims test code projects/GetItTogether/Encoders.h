// Encoders.h

#ifndef _ENCODERS_h
#define _ENCODERS_h

#include "Motors.h"
#include "driver/pcnt.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "esp32-hal.h"

typedef struct Encoder_Settings {
	int pin;                             // GPIO pin of the encoder input signal
	gpio_num_t gpioNum;                  // gpio num
	mcpwm_capture_on_edge_t edgeCapture; // which edge detection to use, pos/neg
	pcnt_unit_t pcntUnit;                // which PCNT unit will the encoder input to
};

typedef struct RPMS {
	int GroundSpeedCount;
	float FL_RPM;
	float FR_RPM;
	float BL_RPM;
	float BR_RPM;
	Movement FL_Wheel_movement;
	Movement FR_Wheel_movement;
	Movement BL_Wheel_movement;
	Movement BR_Wheel_movement;
};


void InitEncoders(void (*LFintISR) (void), void (*RFintISR) (void), void (*RRintISR) (void), void (*LRintISR) (void));
void TimerInterruptInit(void (*intFunc) (void));
RPMS GetRPMS();

#endif
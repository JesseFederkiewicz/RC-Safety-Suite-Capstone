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
};

typedef struct RPMS {
	float FL_RPM;
	float FR_RPM;
	float BL_RPM;
	float BR_RPM;
};

void InitEncoders();
void TimerInterruptInit(void (*intFunc) (void));
RPMS GetRPMS();

#endif
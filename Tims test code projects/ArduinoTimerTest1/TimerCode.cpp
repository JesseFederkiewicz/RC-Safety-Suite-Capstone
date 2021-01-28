/*
 Name:		TimerCode.c
 Created:	1/28/2021
 Author:	Tim/Jesse
*/
#include "TimerCode.h"
#include "esp32-hal.h"
#include "HardwareSerial.h"
#include "driver/timer.h"
#include "arduino.h"

volatile bool intFlag = false; // flag for use in main for actual code to run every interrupt interval

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // used for syncing main and isr, ignore this red squiggle, still works

// timer ISR, trips a flag to use in main
void IRAM_ATTR TimerInt()
{
	portENTER_CRITICAL_ISR(&timerMux);
	intFlag = true;
	portEXIT_CRITICAL_ISR(&timerMux);
}

void Main() 
{
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
		Enable alarm
		First Param:	Which timer, hw_timer_t (timer)
	*/
	timerAlarmEnable(timer);


	int totalInterruptCounter=0;

	for (;;) {
		if (intFlag) {

			portENTER_CRITICAL(&timerMux);
			intFlag = false;
			portEXIT_CRITICAL(&timerMux);

			totalInterruptCounter++;

			Serial.print("An interrupt as occurred. Total number: ");
			Serial.println(totalInterruptCounter);

		}
	}
}
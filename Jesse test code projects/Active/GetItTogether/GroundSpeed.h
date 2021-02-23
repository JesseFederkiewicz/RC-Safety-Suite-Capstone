// GroundSpeed.h

#ifndef _GROUNDSPEED_h
#define _GROUNDSPEED_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Encoders.h"

void GroundSpeedSensorInit();
int16_t GetGroundSpeedCounterVal();

#endif


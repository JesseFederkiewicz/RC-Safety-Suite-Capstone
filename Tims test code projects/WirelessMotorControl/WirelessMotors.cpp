///////////////////////////////////////////////////////////////////////////////////////
// File:        WirelessMotors.cpp
// Author:      Tim Hachey
// Description: now that we have wifi/database communication, 
//				lets put it together and control the motors from the web!
//
//				Lots of debugging, fix was heap space shown in this file
///////////////////////////////////////////////////////////////////////////////////////
#include "WirelessMotors.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"
#include "HardwareSerial.h"
#include "driver/mcpwm.h"
#include "esp32-hal.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "arduino.h"
#include "driver/pcnt.h"
#include <WiFi.h>

//// timer interrupt stuff
static volatile bool intFlag = false; // flag for use in main for actual code to run every interrupt interval
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED; // used for syncing main and isr, ignore this red squiggle, still works

MotorDirection _currentLeftDirection = Forward;
MotorDirection _currentRightDirection = Reverse;

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
	gpio_num_t gpioNum;                  // gpio num
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

void PCNTSetup(Motor_Settings m1, Motor_Settings m2, Motor_Settings m3, Motor_Settings m4)
{
	pcnt_config_t m1Enc;
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

	pcnt_config_t m2Enc;
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

	pcnt_config_t m3Enc;
	m1Enc.pulse_gpio_num = m3.encoder.pin;   // Encoder input pin (GPIO12)
	m1Enc.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	m1Enc.unit = PCNT_UNIT_2;                // motor 1 encoder readings using pcnt unit 0
	m1Enc.channel = PCNT_CHANNEL_0;          // using unit 0 channel 0
	m1Enc.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	m1Enc.neg_mode = PCNT_COUNT_INC;	     // 
	m1Enc.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m1Enc.hctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m1Enc.counter_h_lim = INT16_MAX;
	m1Enc.counter_l_lim = INT16_MIN;

	pcnt_config_t m4Enc;
	m2Enc.pulse_gpio_num = m4.encoder.pin;   // Encoder input pin (GPIO14)
	m2Enc.ctrl_gpio_num = PCNT_PIN_NOT_USED; // control pin not used
	m2Enc.unit = PCNT_UNIT_3;				 // motor 2 encoder readings using pcnt unit 1
	m2Enc.channel = PCNT_CHANNEL_0;          // using unit 1 channel 0
	m2Enc.pos_mode = PCNT_COUNT_INC;         // Count Rising-Edges and falling edges
	m2Enc.neg_mode = PCNT_COUNT_INC;	     // 
	m2Enc.lctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m2Enc.hctrl_mode = PCNT_MODE_KEEP;       // Control mode: won't change counter mode
	m2Enc.counter_h_lim = INT16_MAX;
	m2Enc.counter_l_lim = INT16_MIN;


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

class MotorDuty
{
public:
	float frontLeftMotorDuty;
	float frontRightMotorDuty;
	float backLeftMotorDuty;
	float backRightMotorDuty;
};

MotorDuty SimpleSteering(int angle, int speedIn)
{
	// since first 40% of duty cycle doesnt turn motors,
	// convert % speed to range of 40-100 for more precise control
	// out = ((60/100) * in) + 40

	float speed = (0.6 * speedIn ) + 40.0;

	MotorDuty motorSetting;
	//Serial.println("Into MotorDuty");
	//Serial.print("speed in ");
	//Serial.println(speedIn);
	//Serial.print("angle");
	//Serial.println(angle);
	//Serial.print("speed");
	//Serial.println(speed);

	//Go forward
	if (angle > -10 && angle < 10)
	{
		gpio_set_level(GPIO_NUM_4, Forward);//left motor	//0 forward 1 back
		gpio_set_level(GPIO_NUM_21, Forward);//right motor
		motorSetting.frontLeftMotorDuty = speed;
		motorSetting.frontRightMotorDuty = speed;
		motorSetting.backLeftMotorDuty = speed;
		motorSetting.backRightMotorDuty = speed;
	}

	//Burn right
	else if (angle > 80 && angle < 100)
	{
		gpio_set_level(GPIO_NUM_4, Forward);//left motor	//0 forward 1 back
		gpio_set_level(GPIO_NUM_21, Reverse);//right motor
		motorSetting.frontLeftMotorDuty = speed;
		motorSetting.frontRightMotorDuty = speed;
		motorSetting.backLeftMotorDuty = speed;
		motorSetting.backRightMotorDuty = speed;
	}

	//Burn left
	else if (angle < -80 && angle > -100)
	{
		gpio_set_level(GPIO_NUM_4, Reverse);//left motor	//0 forward 1 back
		gpio_set_level(GPIO_NUM_21, Forward);//right motor
		motorSetting.frontLeftMotorDuty = speed;
		motorSetting.frontRightMotorDuty = speed;
		motorSetting.backLeftMotorDuty = speed;
		motorSetting.backRightMotorDuty = speed;
	}

	//Go backward
	else if (angle < -110 || angle > 110)
	{
		gpio_set_level(GPIO_NUM_4, Reverse);//left motor	//0 forward 1 back
		gpio_set_level(GPIO_NUM_21, Reverse);//right motor
		motorSetting.frontLeftMotorDuty = speed;
		motorSetting.frontRightMotorDuty = speed;
		motorSetting.backLeftMotorDuty = speed;
		motorSetting.backRightMotorDuty = speed;
	}

	//Unknown state condition, stop it
	else
	{
		//gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_4));
		//gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_21));
		motorSetting.frontLeftMotorDuty = 0;
		motorSetting.frontRightMotorDuty = 0;
		motorSetting.backLeftMotorDuty = 0;
		motorSetting.backRightMotorDuty = 0;
	}

	//if stop
	if (speed == 0)
	{
		//gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_4));
		//gpio_set_level(GPIO_NUM_4, !gpio_get_level(GPIO_NUM_21));
		motorSetting.frontLeftMotorDuty = 0;
		motorSetting.frontRightMotorDuty = 0;
		motorSetting.backLeftMotorDuty = 0;
		motorSetting.backRightMotorDuty = 0;
	}

	return motorSetting;
}


void Main()
{
	// motor one configs
	Motor_Settings frontLeftMotor;
	frontLeftMotor.pwm.unit = MCPWM_UNIT_0;               // frontLeftMotor using pwm unit 0
	frontLeftMotor.pwm.timer = MCPWM_TIMER_0;             // frontLeftMotor using unit 0 timer 0
	frontLeftMotor.pwm.opOut = MCPWM_OPR_A;               // frontLeftMotor on operator 0 output A
	frontLeftMotor.pwm.signal = MCPWM0A;                  // sort of the same as above, needed for gpio_init
	frontLeftMotor.pwm.pin = 22;                          // pwm for frontLeftMotor on pin 22
	frontLeftMotor.encoder.pin = 34;                      // encoder input on pin 34
	frontLeftMotor.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	frontLeftMotor.encoder.gpioNum = GPIO_NUM_34;        // gpio num should match pin num

	// motor two configs
	Motor_Settings frontRightMotor;
	frontRightMotor.pwm.unit = frontLeftMotor.pwm.unit;    // frontRightMotor shares unit with frontLeftMotor
	frontRightMotor.pwm.timer = frontLeftMotor.pwm.timer;  // frontRightMotor shares timer with frontLeftMotor
	frontRightMotor.pwm.opOut = MCPWM_OPR_B;               // frontRightMotor on operator 0 output B
	frontRightMotor.pwm.signal = MCPWM0B;                  // needed for gpio_init
	frontRightMotor.pwm.pin = 23;		                  // pwm for frontLeftMotor on pin 23
	frontRightMotor.encoder.pin = 35;                      // encoder input on pin 35
	frontRightMotor.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	frontRightMotor.encoder.gpioNum = GPIO_NUM_35;        // gpio num should match pin num

	// motor three configs
	Motor_Settings backLeftMotor;
	backLeftMotor.pwm.unit = MCPWM_UNIT_1;                // frontRightMotor shares unit with frontLeftMotor
	backLeftMotor.pwm.timer = MCPWM_TIMER_0;              // frontRightMotor shares timer with frontLeftMotor
	backLeftMotor.pwm.opOut = MCPWM_OPR_A;               // frontRightMotor on operator 0 output B
	backLeftMotor.pwm.signal = MCPWM0A;                  // needed for gpio_init
	backLeftMotor.pwm.pin = 15;		                    // pwm for frontLeftMotor on pin 23
	backLeftMotor.encoder.pin = 12;                      // encoder input on pin 35
	backLeftMotor.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	backLeftMotor.encoder.gpioNum = GPIO_NUM_12;        // gpio num should match pin num

	// motor four configs
	Motor_Settings backRightMotor;
	backRightMotor.pwm.unit = backLeftMotor.pwm.unit;     // frontRightMotor shares unit with frontLeftMotor
	backRightMotor.pwm.timer = backLeftMotor.pwm.timer;   // frontRightMotor shares timer with frontLeftMotor
	backRightMotor.pwm.opOut = MCPWM_OPR_B;               // frontRightMotor on operator 0 output B
	backRightMotor.pwm.signal = MCPWM0B;                  // needed for gpio_init
	backRightMotor.pwm.pin = 2;			                  // pwm for frontLeftMotor on pin 23
	backRightMotor.encoder.pin = 14;                      // encoder input on pin 35
	backRightMotor.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	backRightMotor.encoder.gpioNum = GPIO_NUM_14;        // gpio num should match pin num

	// config for pwm unit 0 timer 0 
	mcpwm_config_t pwmconf;
	pwmconf.frequency = 10000;				 // timer freq 10k
	pwmconf.duty_mode = MCPWM_DUTY_MODE_0;   // positive duty cycle
	pwmconf.counter_mode = MCPWM_UP_COUNTER; // timer up counter
	pwmconf.cmpr_a = 0;						 // initial duty of 0
	pwmconf.cmpr_b = 0;

	// run setup for each motor
	PWMSetup(frontLeftMotor, &pwmconf);
	PWMSetup(frontRightMotor, &pwmconf);
	PWMSetup(backLeftMotor, &pwmconf);
	PWMSetup(backRightMotor, &pwmconf);


	// setup gpio pin 21 for rightDirection control
	gpio_num_t dirPinRight = GPIO_NUM_4;
	gpio_config_t dirConfigRight;
	dirConfigRight.intr_type = GPIO_INTR_DISABLE;
	dirConfigRight.mode = GPIO_MODE_OUTPUT;
	dirConfigRight.pin_bit_mask = 0b10000; // bit #4 for pin 4
	dirConfigRight.pull_down_en = GPIO_PULLDOWN_ENABLE;
	dirConfigRight.pull_up_en = GPIO_PULLUP_ENABLE;

	// setup gpio pin 4 for leftDirection control
	gpio_num_t dirPinLeft = GPIO_NUM_21;
	gpio_config_t dirConfigLeft;
	dirConfigLeft.intr_type = GPIO_INTR_DISABLE;
	dirConfigLeft.mode = GPIO_MODE_OUTPUT;
	dirConfigLeft.pin_bit_mask = 0b1000000000000000000000;// bit #21 for pin 21 
	dirConfigLeft.pull_down_en = GPIO_PULLDOWN_ENABLE;
	dirConfigLeft.pull_up_en = GPIO_PULLUP_ENABLE;

	gpio_config(&dirConfigRight);
	gpio_config(&dirConfigLeft);


	// set gpio Forward for now
	int rightDirection = Forward;
	int leftDirection = rightDirection;
	gpio_set_level(GPIO_NUM_21, leftDirection);
	gpio_set_level(GPIO_NUM_4, rightDirection);

	//// testing encoder readings
	//uint encRead1 = 0;
	//uint encRead2 = 0;
	//uint encRead3 = 0;
	//uint encRead4 = 0;

	 //timer/encoder setup:

	//const int timerPrescale = 80;
	//const int timerClk = 80000000 / timerPrescale;
	//const int intTriggerPeriod_ms = 50;

	//PCNTSetup(frontLeftMotor, frontRightMotor, backLeftMotor, backRightMotor);

	//hw_timer_t* timer = NULL;

	///*
	//	set up initial timer configs
	//	First Param:	What timer to use (0)
	//	Second Param:	Prescale (80) allows 1Mhz clock
	//	Third Param:	Count mode (true) count up mode.
	//	Return:			hw_timer_t (timer)
	// */
	//timer = timerBegin(0, timerPrescale, true);

	///*
	//	Attach Timer to ISR
	//	First Param:	Which timer, hw_timer_t (timer)
	//	Second Param:	Callback method for ISR (TimerInt)
	//	Third Param:	Edge: (true)
	//*/
	//timerAttachInterrupt(timer, &TimerInt, true);

	///*
	//	How often to trigger alarm
	//	First Param:	Which timer, hw_timer_t (timer)
	//	Second Param:	Timer counter value when interupt triggers, currently triggering every 1s (1000000) (1Mhz / 1000000)
	//	Third Param:	Reset interrupt flag and timer counter (true)
	//*/
	//timerAlarmWrite(timer, intTriggerPeriod_ms * 1000, true);

	///*
	//	Enable alarm (interrupt)
	//	First Param:	Which timer, hw_timer_t (timer)
	//*/
	//timerAlarmEnable(timer);

	//int16_t enc_count1;
	//int16_t enc_count2;
	//int16_t enc_count3;
	//int16_t enc_count4;


	char* jessessidHOT = "Unhackable II";
	const char* jessepasswordHOT = "plsdontguess";
	char* jessessid = "Cappy";
	const char* jessepassword = "ThisIs@nAdequateP@ss123";
	char* timssid = "hachey wifi";
	const char* timpassword = "38hachey";
	const char* webService = "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php";
	const char* server = "thor.net.nait.ca";

	//Serial.begin(115200);

	WiFi.begin(timssid, timpassword);

	//Serial.print("Connecting");

	while (WiFi.status() != WL_CONNECTED) {
		delay(200);
		//Serial.print(".");
	}
	//Serial.println();
	//Serial.print("Connected, IP address: ");
	//Serial.println(WiFi.localIP());


	unsigned int timeStamp = 1;
	HTTPClient http;
	http.begin(webService);
	http.setReuse(true);

	for (;;) {

		int angleIn = 0;			
		int speedInPercent = 0;	
										
		//HTTPClient http;				
		//Begin webservice call
		//http.begin(webService);

		//Also check if connected to the internet//	
		if (WiFi.status() == WL_CONNECTED) {

			//Add header to http POST
			http.addHeader("Content-Type", "application/x-www-form-urlencoded");

			int httpResponseCode = http.POST("action=GrabWebToCar&carID=1");

			//If good call
			if (httpResponseCode > 0)
			{
				//Load response data
				String payload = http.getString();
				//String payload = http->getString();


				//Parse response into our boy jason
				JSONVar jason = JSON.parse(payload);

				//Get data if timestamp has changed
				if (timeStamp != atoi(jason["timeStamp"]) && atoi(jason["timeStamp"]) != 0)
				{
					angleIn = atoi(jason["angleIn"]);
					speedInPercent = atoi(jason["intendedSpeed"]);
					timeStamp = atoi(jason["timeStamp"]);

					//likely want to count missed pings here
				}

				MotorDuty motorData = SimpleSteering(angleIn, speedInPercent);

				//Set motor duty
				mcpwm_set_duty(frontLeftMotor.pwm.unit, frontLeftMotor.pwm.timer, frontLeftMotor.pwm.opOut, motorData.frontLeftMotorDuty);
				mcpwm_set_duty(frontRightMotor.pwm.unit, frontRightMotor.pwm.timer, frontRightMotor.pwm.opOut, motorData.frontRightMotorDuty);
				mcpwm_set_duty(backLeftMotor.pwm.unit, backLeftMotor.pwm.timer, backLeftMotor.pwm.opOut, motorData.backLeftMotorDuty);
				mcpwm_set_duty(backRightMotor.pwm.unit, backRightMotor.pwm.timer, backRightMotor.pwm.opOut, motorData.backRightMotorDuty);
			}
		}
		else
		{
			WiFi.begin(timssid, timpassword);
			while (WiFi.status() != WL_CONNECTED) {
			}
		}

		//http.end();

		//// currently reading values every second with interrupt timer
		//if (intFlag)
		//{
		//	/*
		//		RPM = (encoder count)/(211.2 counts per revolution)*(60 rpm)*(2 (only using 1 encoder signal))*(timerClk/intTriggerPeriodUs);
		//		motor specs: https://www.pololu.com/product/4861
		//	*/
		//	pcnt_get_counter_value(PCNT_UNIT_0, &enc_count1);
		//	pcnt_get_counter_value(PCNT_UNIT_1, &enc_count2);
		//	pcnt_get_counter_value(PCNT_UNIT_2, &enc_count3);
		//	pcnt_get_counter_value(PCNT_UNIT_3, &enc_count4);

		//	//Serial.print("RPM1:");
		//	//Serial.println((float)enc_count1 / 211.2 * 120.0 * (float)(timerClk / intTriggerPeriod_ms));

		//	//Serial.print("RPM2:");
		//	//Serial.println((float)enc_count2 / 211.2 * 120.0 * (float)(timerClk / intTriggerPeriod_ms));

		//	//Serial.print("RPM3:");
		//	//Serial.println((float)enc_count3 / 211.2 * 120.0 * (float)(timerClk / intTriggerPeriod_ms));

		//	//Serial.print("RPM4:");
		//	//Serial.println((float)enc_count4 / 211.2 * 120.0 * (float)(timerClk / intTriggerPeriod_ms));

		//	pcnt_counter_clear(PCNT_UNIT_0);
		//	pcnt_counter_clear(PCNT_UNIT_1);
		//	pcnt_counter_clear(PCNT_UNIT_2);
		//	pcnt_counter_clear(PCNT_UNIT_3);

		//	portENTER_CRITICAL(&timerMux);
		//	intFlag = false;
		//	portEXIT_CRITICAL(&timerMux);
		//}

	}
}

//timer ISR, trips a flag to use in main
void IRAM_ATTR TimerInt()
{
	portENTER_CRITICAL_ISR(&timerMux);
	intFlag = true;
	portEXIT_CRITICAL_ISR(&timerMux);
}


///////////////////////////////////////////////////////////////////////////////////////
// File:        WirelessMotors.cpp
// Author:      Tim Hachey
// Description: now that we have wifi/database communication, 
//				lets put it together and control the motors from the web!
///////////////////////////////////////////////////////////////////////////////////////
#include "WirelessMotors.h"
#include "WiFi.h"
#include "HTTPClient.h"
#include "Arduino_JSON.h"
#include "esp_wifi.h"
#include "HardwareSerial.h"
#include "driver/mcpwm.h"
#include "esp32-hal.h"
#include "driver/gpio.h"
#include "driver/timer.h"
#include "arduino.h"

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
	int preScale;                        // prescaler for the capture timer
	mcpwm_capture_signal_t capSignal;    // the capture signal to use
	mcpwm_io_signals_t signal;           // specific io signal
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

	// attach capture signal input to gpio pin
	mcpwm_gpio_init(m.pwm.unit, m.encoder.signal, m.encoder.pin);

	// init the pwm unit, not actually needed for each motor, only for each unit... but oh well
	mcpwm_init(m.pwm.unit, m.pwm.timer, unitConf);

	// enable/init PWM encoder capturing
	mcpwm_capture_enable(m.pwm.unit, m.encoder.capSignal, m.encoder.edgeCapture, m.encoder.preScale);
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
	m1.encoder.signal = MCPWM_CAP_0;          // encoder io signal
	m1.encoder.capSignal = MCPWM_SELECT_CAP0; // same as above but for mcpwm_capture_enable()
	m1.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	m1.encoder.preScale = 1;                  // no scaling of capture clock for fast reading response

	// motor two configs
	Motor_Settings m2;
	m2.pwm.unit = m1.pwm.unit;                // m2 shares unit with m1
	m2.pwm.timer = m1.pwm.timer;              // m2 shares timer with m1
	m2.pwm.opOut = MCPWM_OPR_B;               // m2 on operator 0 output B
	m2.pwm.signal = MCPWM0B;                  // needed for gpio_init
	m2.pwm.pin = 23;		                  // pwm for m1 on pin 23
	m2.encoder.pin = 35;                      // encoder input on pin 35
	m2.encoder.signal = MCPWM_CAP_1;		  // encoder io signal
	m2.encoder.capSignal = MCPWM_SELECT_CAP1; // same as above but for mcpwm_capture_enable()
	m2.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	m2.encoder.preScale = 1;                  // no scaling of capture clock for fast reading response

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

	// testing encoder readings
	uint encRead1 = 0;
	uint encRead2 = 0;

	// setup gpio pin 1 for direction control
	gpio_num_t dirPin = GPIO_NUM_1;
	gpio_config_t dirConfig;
	dirConfig.intr_type = GPIO_INTR_DISABLE;
	dirConfig.mode = GPIO_MODE_OUTPUT;
	dirConfig.pin_bit_mask = 0b10; // bit #1 for pin 1
	dirConfig.pull_down_en = GPIO_PULLDOWN_ENABLE;
	dirConfig.pull_up_en = GPIO_PULLUP_ENABLE;

	gpio_config(&dirConfig);

	// set gpio 1 high for now
	int direction = 1;
	gpio_set_level(GPIO_NUM_1, direction);

	char* ssid = "hachey wifi";
	const char* password = "38hachey";
	const char* webService = "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php";
	const char* postArgs = "action=GrabXYTimeStamp&carID=1";
	const char* server = "thor.net.nait.ca";

	Serial.begin(115200);
	WiFi.begin(ssid, password);

	Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println();

	Serial.print("Connected, IP address: ");
	Serial.println(WiFi.localIP());

	HTTPClient http;

	JSONVar jason;

	for (;;) {
		if (WiFi.status() == WL_CONNECTED) {

			if (!http.connected())
				http.begin(webService);

			http.addHeader("Content-Type", "application/x-www-form-urlencoded");
			int httpResponseCode = http.POST(postArgs);

			if (httpResponseCode > 0)
			{
				Serial.println("HTTP Response code: ");
				Serial.println(httpResponseCode);

				String payload = http.getString();
				delay(1);
				Serial.println("Payload: ");
				jason = JSON.parse(payload);
				Serial.println(jason["xCoord"]);
				Serial.println(jason["yCoord"]);
				Serial.println(jason["timeStamp"]);
			}

			char* xCord = "50";
			char* yCord = "50";

			Serial.println("TEST1");

			String one = JSON.stringify(jason["xCoord"]);
			String two = JSON.stringify(jason["yCoord"]);

			Serial.println("TEST2");

		/*	one.toCharArray(xCord, sizeof(char) * 3, 0);
			two.toCharArray(yCord, sizeof(char) * 3, 0);*/

			Serial.println("TEST3");

			int duty1 = strtol(xCord, nullptr, 10);
			int duty2 = strtol(yCord, nullptr, 10);

			/*int duty1 = atoi(XCord);
			int duty2 = atoi(YCord)*/

			Serial.print("String 1: ");
			Serial.println(one);
			Serial.print("String 2: ");
			Serial.println(two);

			Serial.print("xCord: ");
			Serial.println(xCord);
			Serial.print("yCord: ");
			Serial.println(yCord);
			
			Serial.print("Duty1: ");
			Serial.println(duty1);
			Serial.print("Duty2: ");
			Serial.println(duty2);

			Serial.println("TEST4");

			delay(100);

			//mcpwm_set_duty(m1.pwm.unit, m1.pwm.timer, m1.pwm.opOut, duty1);
			//mcpwm_set_duty(m2.pwm.unit, m2.pwm.timer, m2.pwm.opOut, duty2);

			delay(100);
		}		
	}
}
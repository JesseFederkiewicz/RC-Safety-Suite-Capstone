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

class MotorDuty
{
	public:
		int frontLeftMotorDuty;
		int frontRightMotorDuty;
		int backLeftMotorDuty;
		int backRightMotorDuty;
};

MotorDuty SimpleSteering(int dbX, int dbY)
{
	MotorDuty motorSetting;

	if (dbX > 0)
	{
		motorSetting.frontLeftMotorDuty = 100;
		motorSetting.frontRightMotorDuty = 0;
		motorSetting.backLeftMotorDuty = 100;
		motorSetting.backRightMotorDuty = 0;
	}

	else if (dbX < 0)
	{
		motorSetting.frontLeftMotorDuty = 0;
		motorSetting.frontRightMotorDuty = 100;
		motorSetting.backLeftMotorDuty = 0;
		motorSetting.backRightMotorDuty = 100;
	}

	else
	{
		motorSetting.frontLeftMotorDuty = 0;
		motorSetting.frontRightMotorDuty = 0;
		motorSetting.backLeftMotorDuty = 0;
		motorSetting.backRightMotorDuty = 0;
	}

	if (dbY == 100)
	{
		motorSetting.frontLeftMotorDuty = 100;
		motorSetting.frontRightMotorDuty = 100;
		motorSetting.backLeftMotorDuty = 100;
		motorSetting.backRightMotorDuty = 100;
	}
	//else
	//{
	//	motorSetting.frontLeftMotorDuty = 0;
	//	motorSetting.frontRightMotorDuty = 0;
	//	motorSetting.backLeftMotorDuty = 0;
	//	motorSetting.backRightMotorDuty = 0;
	//}


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
	frontLeftMotor.encoder.signal = MCPWM_CAP_0;          // encoder io signal
	frontLeftMotor.encoder.capSignal = MCPWM_SELECT_CAP0; // same as above but for mcpwm_capture_enable()
	frontLeftMotor.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	frontLeftMotor.encoder.preScale = 1;                  // no scaling of capture clock for fast reading response

	// motor two configs
	Motor_Settings frontRightMotor;
	frontRightMotor.pwm.unit = frontLeftMotor.pwm.unit;                // frontRightMotor shares unit with frontLeftMotor
	frontRightMotor.pwm.timer = frontLeftMotor.pwm.timer;              // frontRightMotor shares timer with frontLeftMotor
	frontRightMotor.pwm.opOut = MCPWM_OPR_B;               // frontRightMotor on operator 0 output B
	frontRightMotor.pwm.signal = MCPWM0B;                  // needed for gpio_init
	frontRightMotor.pwm.pin = 23;		                  // pwm for frontLeftMotor on pin 23
	frontRightMotor.encoder.pin = 35;                      // encoder input on pin 35
	frontRightMotor.encoder.signal = MCPWM_CAP_1;		  // encoder io signal
	frontRightMotor.encoder.capSignal = MCPWM_SELECT_CAP1; // same as above but for mcpwm_capture_enable()
	frontRightMotor.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	frontRightMotor.encoder.preScale = 1;                  // no scaling of capture clock for fast reading response

	// motor three configs
	Motor_Settings backLeftMotor;
	backLeftMotor.pwm.unit = MCPWM_UNIT_1;                // frontRightMotor shares unit with frontLeftMotor
	backLeftMotor.pwm.timer = MCPWM_TIMER_0;              // frontRightMotor shares timer with frontLeftMotor
	backLeftMotor.pwm.opOut = MCPWM_OPR_A;               // frontRightMotor on operator 0 output B
	backLeftMotor.pwm.signal = MCPWM0A;                  // needed for gpio_init
	backLeftMotor.pwm.pin = 15;		                  // pwm for frontLeftMotor on pin 23
	backLeftMotor.encoder.pin = 12;                      // encoder input on pin 35
	backLeftMotor.encoder.signal = MCPWM_CAP_1;		  // encoder io signal
	backLeftMotor.encoder.capSignal = MCPWM_SELECT_CAP1; // same as above but for mcpwm_capture_enable()
	backLeftMotor.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	backLeftMotor.encoder.preScale = 1;                  // no scaling of capture clock for fast reading response

	// motor four configs
	Motor_Settings backRightMotor;
	backRightMotor.pwm.unit = backLeftMotor.pwm.unit;                // frontRightMotor shares unit with frontLeftMotor
	backRightMotor.pwm.timer = backLeftMotor.pwm.timer;              // frontRightMotor shares timer with frontLeftMotor
	backRightMotor.pwm.opOut = MCPWM_OPR_B;               // frontRightMotor on operator 0 output B
	backRightMotor.pwm.signal = MCPWM0B;                  // needed for gpio_init
	backRightMotor.pwm.pin = 2;			                  // pwm for frontLeftMotor on pin 23
	backRightMotor.encoder.pin = 14;                      // encoder input on pin 35
	backRightMotor.encoder.signal = MCPWM_CAP_0;		  // encoder io signal
	backRightMotor.encoder.capSignal = MCPWM_SELECT_CAP0; // same as above but for mcpwm_capture_enable()
	backRightMotor.encoder.edgeCapture = MCPWM_POS_EDGE;  // capture positive edges
	backRightMotor.encoder.preScale = 1;                  // no scaling of capture clock for fast reading response

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

	// testing encoder readings
	uint encRead1 = 0;
	uint encRead2 = 0;
	uint encRead3 = 0;
	uint encRead4 = 0;

	// setup gpio pin 1 for rightDirection control
	gpio_num_t dirPinRight = GPIO_NUM_1;
	gpio_config_t dirConfigRight;
	dirConfigRight.intr_type = GPIO_INTR_DISABLE;
	dirConfigRight.mode = GPIO_MODE_OUTPUT;
	dirConfigRight.pin_bit_mask = 0b10; // bit #1 for pin 1
	dirConfigRight.pull_down_en = GPIO_PULLDOWN_ENABLE;
	dirConfigRight.pull_up_en = GPIO_PULLUP_ENABLE;

	// setup gpio pin 32 for leftDirectionDirection control
	gpio_num_t dirPinLeft = GPIO_NUM_0;
	gpio_config_t dirConfigLeft;
	dirConfigLeft.intr_type = GPIO_INTR_DISABLE;
	dirConfigLeft.mode = GPIO_MODE_OUTPUT;
	dirConfigLeft.pin_bit_mask = 0b01; // bit #1 for pin 1
	dirConfigLeft.pull_down_en = GPIO_PULLDOWN_ENABLE;
	dirConfigLeft.pull_up_en = GPIO_PULLUP_ENABLE;

	gpio_config(&dirConfigRight);
	gpio_config(&dirConfigLeft);

	// set gpio 1 high for now
	int rightDirection = 1;
	int leftDirection = rightDirection;
	gpio_set_level(GPIO_NUM_1, rightDirection);
	gpio_set_level(GPIO_NUM_32, leftDirection);

	char* ssid = "Cappy";
	const char* password = "ThisIs@nAdequateP@ss123";
	const char* webService = "https://thor.net.nait.ca/~jfederki/cmpe2500/Rc_Safety_Suite/Main%20Web/webservice.php";
	const char* postArgs = "action=GrabXYTimeStamp&carID=1";
	const char* server = "thor.net.nait.ca";

	Serial.begin(115200);
	WiFi.begin(ssid, password);

	//Serial.print("Connecting");
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		//Serial.print(".");
	}
	//Serial.println();

	//Serial.print("Connected, IP address: ");
	//Serial.println(WiFi.localIP());

	HTTPClient http;

	JSONVar jason;

	int xCoord = 0;
	int yCoord = 0;
	unsigned int timeStamp = 0;

	for (;;) {
		if (WiFi.status() == WL_CONNECTED) {

			if (!http.connected())
				http.begin(webService);

			http.addHeader("Content-Type", "application/x-www-form-urlencoded");
			delay(2);
			int httpResponseCode = http.POST(postArgs);
			delay(200);

			if (httpResponseCode > 0)
			{
				Serial.println(httpResponseCode);
				String payload = http.getString();		//causing crash and reboot ocasionally (try catch no fix)
				Serial.println(payload);
				jason = JSON.parse(payload);

				if (timeStamp != atoi(jason["timeStamp"]) && atoi(jason["timeStamp"]) != 0)
				{
					Serial.println(atoi(jason["timeStamp"]));
					xCoord = atoi(jason["xCoord"]);
					yCoord = atoi(jason["yCoord"]);
					timeStamp = atoi(jason["timeStamp"]);
					//Serial.println(xCoord);
					//Serial.println(yCoord);
				}
								
			}

			MotorDuty motorData = SimpleSteering(xCoord, yCoord);

			Serial.println(motorData.frontLeftMotorDuty);
			Serial.println(motorData.frontRightMotorDuty);
			Serial.println(motorData.backLeftMotorDuty);
			Serial.println(motorData.backRightMotorDuty);
			Serial.println();

			mcpwm_set_duty(frontLeftMotor.pwm.unit, frontLeftMotor.pwm.timer, frontLeftMotor.pwm.opOut, motorData.frontLeftMotorDuty);
			mcpwm_set_duty(frontRightMotor.pwm.unit, frontRightMotor.pwm.timer, frontRightMotor.pwm.opOut, motorData.frontRightMotorDuty);			
			mcpwm_set_duty(backLeftMotor.pwm.unit, backLeftMotor.pwm.timer, backLeftMotor.pwm.opOut, motorData.backLeftMotorDuty);
			mcpwm_set_duty(backRightMotor.pwm.unit, backRightMotor.pwm.timer, backRightMotor.pwm.opOut, motorData.backRightMotorDuty);
		}		
	}
}

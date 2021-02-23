/*
 Name:		GroundSpeedSensor.ino
 Created:	2/22/2021 3:34:18 PM
 Author:	Jesse Federkiewicz
*/

#include "./src/MsTimer2.h"

int pbIn = GPIO_NUM_11;                    // Define interrupt 0 that is digital pin 2
int ledOut = 13;                 // Define the indicator LED pin digital pin 13
int number = 0;                    //Interrupt times
volatile int state = LOW;         // Defines the indicator LED state, the default is not bright

void setup()
{
    Serial.begin(9600);
    pinMode(ledOut, OUTPUT);//
    attachInterrupt(pbIn, stateChange, FALLING); // Set the interrupt function, interrupt pin is digital pin D2, interrupt service function is stateChange (), when the D2 power change from high to low , the trigger interrupt.
    MsTimer2::set(1000, Handle); // Set the timer interrupt function, running once Handle() function per 1000ms
    MsTimer2::start();//Start timer interrupt function

}

void loop()
{
    //Serial.println(number); // Printing the number of times of interruption, which is convenient for debugging.
    //delay(1);
    //if (state == HIGH)  //When a moving object is detected, the ledout is automatically closed after the light 2S, the next trigger can be carried out, and No need to reset. Convenient debugging.
    //{
    //    delay(2000);
    //    state = LOW;
    //    digitalWrite(ledOut, state);    //turn off led
    //}
    return;
}


void stateChange()  //Interrupt service function
{
    number++;  //Interrupted once, the number + 1

}

void Handle()   //Timer service function
{
    if (number > 1)  //If in the set of the interrupt time the number more than 1 times, then means have detect moving objects,This value can be adjusted according to the actual situation, which is equivalent to adjust the threshold of detection speed of moving objects.
    {
        state = HIGH;
        digitalWrite(ledOut, state);    //light led
        Serial.println(number);
        number = 0;   //Cleare the number, so that it does not affect the next trigger
    }
    else
        number = 0;   //If in the setting of the interrupt time, the number of the interrupt is not reached the threshold value, it is not detected the moving objects, Cleare the number.
}
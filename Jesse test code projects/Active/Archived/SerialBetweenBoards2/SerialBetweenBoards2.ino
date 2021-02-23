#include <HardwareSerial.h>

void setup()
{
    Serial.begin(115200);
    Serial2.begin(115200);//open the other serial port
}

void loop()
{
    if (Serial2.available()) {
        Serial.printf("%c",Serial2.read());
    }
}
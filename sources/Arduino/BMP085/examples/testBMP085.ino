/*
Serial.print Temperature and pressure read form a BMP085 at 115200 bauds to serial monitor
*/

#include <Wire.h>

#include <BMP085.h>

BMP085 captTP;

void setup()
{
  Serial.begin(115200);
  captTP.begin();
}

void loop()
{
 Serial.print(captTP.getTemperature());
 Serial.print("  ");
 Serial.println(captTP.getPressure());
 delay(200);
}


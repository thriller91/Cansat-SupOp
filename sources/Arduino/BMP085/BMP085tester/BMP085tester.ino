/*
Ce programme marche sur la Mini (sans translateur de niveau)
Branchement:
SDA -> AD4
SCL -> AD5
GND -> GND (alim stab)
VCC -> 3.3V (alim stab)
*/

//Arduino 1.0+ Only
//Arduino 1.0+ Only

/*Based largely on code by  Jim Lindblom

 Get pressure, altitude, and temperature from the BMP085.
 Serial.print it out at 9600 baud to serial monitor.
 */

#include <Wire.h>
#include "BMP085.h"



void setup(){
  Serial.begin(9600);
  Wire.begin();
  Serial.println("BMP085...");
  Serial.print("Calibration...");

  bmp085Calibration();
  Serial.println(" ...done");
}

void loop()
{
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  float atm = pressure / 101325; // "standard atmosphere"
  float altitude = calcAltitude(pressure); //Uncompensated caculation - in Meters 

  Serial.print("Temperature: ");
  Serial.print(temperature, 2); //display 2 decimal places
  Serial.println("deg C");

  Serial.print("Pressure: ");
  Serial.print(pressure, 0); //whole number only.
  Serial.println(" Pa");

  Serial.print("Standard Atmosphere: ");
  Serial.println(atm, 4); //display 4 decimal places

  Serial.print("Altitude: ");
  Serial.print(altitude, 2); //display 2 decimal places
  Serial.println(" M");

  Serial.println();//line break

  delay(1000); //wait a second and get values again.
}



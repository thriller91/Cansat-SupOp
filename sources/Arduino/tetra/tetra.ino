/*
Programme à embarquer sur l'Arduino n°1 du τετραφάρμακος.
TODO
-stériliser le code;
-ajouter les headers définis par Denis pour signer chaque type de données et faciliter la lecture;
-ajouter la commutation du module XBee avec la Arduino n°2.
*/

#include <Wire.h>
#include <SoftwareSerial.h>
#include "BMP085.h"
#include "DHT22.h"
#include "TinyGPS.h"

//BMP085
BMP085 captTP;

//DH22
DHT22 captHT(9);

//ADXL335
const int groundpin = 18;             // analog input pin 4 -- ground
const int powerpin = 19;              // analog input pin 5 -- voltage
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)

//GPS
TinyGPS gps;
SoftwareSerial ss(3, 4); //pins 3(rx) and 4(tx)


void setup(){
	Serial.begin(115200);

        //BMP085
	captTP.begin();

	// ADXL335
	pinMode(groundpin, OUTPUT);
	pinMode(powerpin, OUTPUT);
	digitalWrite(groundpin, LOW); 
	digitalWrite(powerpin, HIGH);
        
        //GPS
        ss.begin(4800);
}

void loop(){

	/*
	Sortie série de la pression et de la température du capteur BMP085
	*/
	Serial.print(captTP.getTemperature());
	Serial.print("  ");
	Serial.println(captTP.getPressure());
	
	/*
	Sortie série de l'humidité et de la température du capteur DH22
	*/
	Serial.print(captHT.getHumidity());
	Serial.print("  ");
	Serial.println(captHT.getTemperatureC());
	
	/*
	Sortie série des accélérations du capteur ADXL335
	*/
	Serial.print(analogRead(xpin));
	Serial.print("\t");
	Serial.print(analogRead(ypin));
	Serial.print("\t");
	Serial.println(analogRead(zpin));

	/*
	Sortie série d'une trame GPS
	*/
        bool newData = false;
        for (unsigned long start = millis(); millis() - start < 1000;){
          while (ss.available()){
            char c = ss.read();
            Serial.write(c); // uncomment this line if you want to see the GPS data flowing
            if (gps.encode(c)) // Did a new valid sentence come in?
              newData = true;
          }
        }

}

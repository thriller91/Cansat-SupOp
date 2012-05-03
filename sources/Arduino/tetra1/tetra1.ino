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

//BMP085
BMP085 captTP;

//DH22
DHT22 captHT(9);


void setup(){
	Serial.begin(115200);

        //BMP085
	captTP.begin();
}

void loop(){

	/*
	Sortie série de la pression et de la température du capteur BMP085
	*/
	Serial.print(captTP.getTemperature());
	Serial.print(" // ");
	Serial.println(captTP.getPressure());
	
	/*
	Sortie série de l'humidité et de la température du capteur DH22
	*/
	Serial.print(captHT.getHumidity());
	Serial.print(" // ");
	Serial.println(captHT.getTemperatureC());

}

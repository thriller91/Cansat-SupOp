/*
Programme de test des composants à embarquer sur l'Arduino Mini n°1 du τετραφάρμακος.
*/

#include <Wire.h>

#include "BMP085.h"
#include "DHT.h"


// RHT03
#define DHTPIN 2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);


void setup(){
	// Communication
	Serial.begin(9600);
	Serial.println("Debut");
	delay(1000);

	//BMP085
	Wire.begin();
	bmp085Calibration();

	//RHT03
	dht.begin();


	while (true){
		/*
		Sortie série de la pression et de la température du capteur BMP085
		*/

		float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
		float pressure = bmp085GetPressure(bmp085ReadUP());
		Serial.print("BMP085: \t T: ");
		Serial.print(temperature, 2); //display 2 decimal places
		Serial.print(" deg C\t");
		Serial.print("P: ");
		Serial.print(pressure, 0); //whole number only.
		Serial.println(" Pa");

		/*
		Sortie série de l'humidité et de la température du capteur DH22
		*/

		float h = dht.readHumidity();
		float t = dht.readTemperature();
		// check if returns are valid, if they are NaN (not a number) then something went wrong!
		if (isnan(t) || isnan(h)) {
			Serial.println("Failed to read from DHT");
		}
		else {
			Serial.print("RHT03: \t Humidity: ");
			Serial.print(h);
			Serial.print(" %\t");
			Serial.print("Temperature: ");
			Serial.print(t);
			Serial.println(" *C");
		}
	}

}

void loop(){

	Serial.println("*[%0%]*");
	delay(10000);


}




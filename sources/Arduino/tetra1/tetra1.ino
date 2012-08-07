/*
Programme à embarquer sur l'Arduino Mini n°1 du τετραφάρμακος.
TODO
-ajouter la LinkSprite n°1;
-gérer la communication avec l'Arduino Mini n°2.
*/

#include <Wire.h>
#include <SD.h>
#include <SoftwareSerial.h>

#include "utils.h"

#include "BMP085.h"
#include "DHT.h"

SoftwareSerial XBee(XBEE_RX,XBEE_TX);
// Ouverture des fichiers, pour l'Arduino n°1
//OpenFiles Files(0);
//int k=0;

// RHT03
#define DHTPIN 2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);


void setup(){
	// Communication
	Serial.begin(115200);
	XBee.begin(9600);
	Serial.println("Debut");
	XBee.println("Debut");
	pinMode(SWITCH_A1,OUTPUT);
	pinMode(SWITCH_A2,OUTPUT);
	digitalWrite(SWITCH_A1,HIGH);
	digitalWrite(SWITCH_A2,HIGH);

	//BMP085
	Serial.println("BMP085");
	Wire.begin();
	bmp085Calibration();

	//RHT03
	Serial.println("RHT03");
	dht.begin();

}

void loop(){
/*
	while (k<20){
		/*
		Sortie série de la pression et de la température du capteur BMP085
		*/
/*
		float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
		float pressure = bmp085GetPressure(bmp085ReadUP());
		Files.PTH_File.print("BMP085: \t T: ");
		Files.PTH_File.print(temperature, 2); //display 2 decimal places
		Files.PTH_File.print(" deg C\t");
		Files.PTH_File.print("P: ");
		Files.PTH_File.print(pressure, 0); //whole number only.
		Files.PTH_File.println(" Pa");

		/*
		Sortie série de l'humidité et de la température du capteur DH22
		*/
/*
		float h = dht.readHumidity();
		float t = dht.readTemperature();
		// check if returns are valid, if they are NaN (not a number) then something went wrong!
		if (isnan(t) || isnan(h)) {
			Files.PTH_File.println("Failed to read from DHT");
		}
		else {
			Files.PTH_File.print("RHT03: \t Humidity: ");
			Files.PTH_File.print(h);
			Files.PTH_File.print(" %\t");
			Files.PTH_File.print("Temperature: ");
			Files.PTH_File.print(t);
			Files.PTH_File.println(" *C");
			Serial.println(k);
		}
		k++;
	}
	Files.PTH_File.close();
	*/
	XBee.println("Fin");
	Serial.println("Fin");
	delay(1000);
}

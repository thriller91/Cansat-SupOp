/*
Programme à embarquer sur l'Arduino Mini n°1 du τετραφάρμακος.
TODO
-gérer la communication avec l'Arduino Mini n°2.
*/

#include <Wire.h>
#include <SD.h>
#include <SoftwareSerial.h>

#include "tetrapharmakos.h"

#include "LinkSprite.h"
#include "BMP085.h"
#include "DHT.h"

SoftwareSerial XBee(XBEE_RX,XBEE_TX);
// Ouverture des fichiers, pour l'Arduino n°1
OpenFiles Files(0);
int i=0;

// LinkSprite Jpeg Camera
LinkSprite Cam;


// RHT03
#define DHTPIN 2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);


void setup(){
	// Communication
	Serial.begin(38400);
	XBee.begin(9600);
	XBee.println("Debut");

	//BMP085
	Wire.begin();
	bmp085Calibration();

	//RHT03
	dht.begin();

	///////////////////////////////////////////// ETAPE 3
	XBee.println("*[%3%]*");

	// LinkSprite
	Cam.Snap();
	///////////////////////////////////////////// ETAPE 2
	XBee.println("*[%2%]*");

	Cam.Save(Files.Cam_File);

	///////////////////////////////////////////// ETAPE 1
	XBee.println("*[%1%]*");

	while (i<20){
		/*
		Sortie série de la pression et de la température du capteur BMP085
		*/

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
		}
		i++;
	}
	Files.PTH_File.close();

}

void loop(){

	///////////////////////////////////////////// ETAPE 0
	XBee.println("*[%0%]*");
	delay(10000);


}




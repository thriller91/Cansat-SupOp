/*
Programme à embarquer sur l'Arduino Mini n°2 du τετραφάρμακος.
TODO
-tester le programme;
-gérer la communication avec l'Arduino Mini n°1.
*/

#include <Wire.h>
#include <SD.h>
#include <SoftwareSerial.h>

#include "tetrapharmakos.h"

#include "LinkSprite.h"
#include "ADXL345.h"
#include "TinyGPS.h"


SoftwareSerial XBee(XBEE_RX,XBEE_TX);
// Ouverture des fichiers, pour l'Arduino n°1
OpenFiles Files(1);
int i=0;

// LinkSprite Jpeg Camera
LinkSprite Cam;


void setup(){
	// Communication
	Serial.begin(38400);

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
}




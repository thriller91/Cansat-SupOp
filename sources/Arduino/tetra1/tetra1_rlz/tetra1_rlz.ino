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

#define IMG_FILENAME "IMG01.JPG"
#define PTH_FILENAME "PTH.TXT"


//SoftwareSerial XBee(XBEE_RX,XBEE_TX);

File PTH_File, Cam_File;
int i=0;

// RHT03
#define DHTPIN 2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);



void setup(){

	// Communication
	Serial.begin(9600);
	CamStart();
	Serial.println("Debut");
	//XBee.begin(9600);
	//XBee.println("Debut");

	// SD Initialisation
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT))
		Serial.println("initialization failed!");

	//BMP085
	Wire.begin();
	bmp085Calibration();

	//RHT03
	dht.begin();

	///////////////////////////////////////////// ETAPE 3
	//XBee.println("*[%3%]*");

	SendTakePhotoCmd();
	Serial.println("Snap!");

	///////////////////////////////////////////// ETAPE 2
	//XBee.println("*[%2%]*");


	if(SD.exists(PTH_FILENAME))
		SD.remove(PTH_FILENAME);
	PTH_File = SD.open(PTH_FILENAME, FILE_WRITE);
	while (i<10){
		/*
		Sortie série de la pression et de la température du capteur BMP085
		*/

		float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
		float pressure = bmp085GetPressure(bmp085ReadUP());
		PTH_File.print("B\t");
		PTH_File.print(temperature, 2); //display 2 decimal places
		PTH_File.print("\t");
		PTH_File.println(pressure, 0); //whole number only.

		/*
		Sortie série de l'humidité et de la température du capteur DH22
		*/

		float h = dht.readHumidity();
		float t = dht.readTemperature();
		// check if returns are valid, if they are NaN (not a number) then something went wrong!
		if (isnan(t) || isnan(h)) {
			PTH_File.println("R\tX\tX");
		}
		else {
			PTH_File.print("R\t");
			PTH_File.print(h);
			PTH_File.print("\t");
			PTH_File.println(t);
		}


		PTH_File.println(millis());
		i++;
		Serial.println(i);
	}
	PTH_File.close();
	Serial.println("PTH_File closed");


	///////////////////////////////////////////// ETAPE 1
	//XBee.println("*[%1%]*");

	if(SD.exists(IMG_FILENAME))
		SD.remove(IMG_FILENAME);
	Cam_File = SD.open(IMG_FILENAME, FILE_WRITE);
	Serial.println("Ouverture du fichier");
	SaveToFile(Cam_File);

	Cam_File.close();
	Serial.println("Cam_File closed");

}

void loop(){

	///////////////////////////////////////////// ETAPE 0
	//XBee.println("*[%0%]*");
	Serial.println("Fin");
	delay(10000);


}




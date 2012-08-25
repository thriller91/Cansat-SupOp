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

#define INTERRUPT_PIN 1


//SoftwareSerial XBee(XBEE_RX,XBEE_TX);

File PTH_File, Cam_File, dataFile;

// RHT03
#define DHTPIN 2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);
bool isnan_t,isnan_h;

// Station au sol
char cmd[3]={'0','0'};

// interrupt
volatile unsigned long start_missions = 0;
volatile boolean start_flag = false;


void setup(){
	// Communication
	Serial.begin(9600);
	CamStart();
	attachInterrupt(INTERRUPT_PIN, start_counter, RISING);

	//XBee.begin(9600);
	//XBee.println("Debut");

	// SD Initialisation
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT)) {
		Serial.println("initialization failed!");
		while(true); // ça sert à rien de continuer
	}

	//BMP085
	Wire.begin();
	bmp085Calibration();

	//RHT03
	dht.begin();


	///////////////////////////////////////////// ETAPE 8
	// L'Éveil du τετραφάρμακος
	Serial.println("*[%8%]*");
	//
	while(true) {
		if(Serial.available()>0) {
			cmd[0] = cmd[1];
			cmd[1] = Serial.read();
		}
		if(cmd[0] == 'G' && cmd[1] == 'f') {
			while(Serial.available()>0)
				Serial.read();
			cmd[0] = '0';
			cmd[1] = '0';
			break;
		}
	}

	///////////////////////////////////////////// ETAPE 7
	// Attente de largage ...
	Serial.println("*[%7%]*");
	//

	while(!start_flag);

	///////////////////////////////////////////// ETAPE 6
	// Largage, le cavalier saute!
	Serial.println("*[%6%]*");
	//



	///////////////////////////////////////////// ETAPE 5
	// Capture d'image (cf. étape 3 pour l'enregistrement)
	Serial.println("*[%5%]*");
	//

	SendTakePhotoCmd();

	///////////////////////////////////////////// ETAPE 4
	// Mesure PTH et enregistrement sur carte µSD
	Serial.println("*[%4%]*");
	//

	if(SD.exists(PTH_FILENAME))
		SD.remove(PTH_FILENAME);
	PTH_File = SD.open(PTH_FILENAME, FILE_WRITE);

	while ((micros()-start_missions) < stop_missions_time) {
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
		isnan_t = isnan(t);
		isnan_h = isnan(h);

		PTH_File.print("R\t");
		if (isnan_t || isnan_h) {
			if (isnan_h && isnan_t) {
				PTH_File.println("NaN\tNaN");
			}
			else if (isnan_t){
				PTH_File.print(h);
				PTH_File.println("\tNaN");
			}
			else {
				PTH_File.print("NaN\t");
				PTH_File.println(t);
			}
		}
		else {
			PTH_File.print(h);
			PTH_File.print("\t");
			PTH_File.println(t);
		}


		PTH_File.println(micros()-start_missions);
	}
	PTH_File.close();

	///////////////////////////////////////////// ETAPE 3
	// Enregistrement du fichier image 1 (IMG01.JPG) sur la carte µSD
	Serial.println("*[%3%]*");
	//

	if(SD.exists(IMG_FILENAME))
		SD.remove(IMG_FILENAME);
	Cam_File = SD.open(IMG_FILENAME, FILE_WRITE);

	SaveToFile(Cam_File);

	Cam_File.close();
	ShutDownCmd();

	///////////////////////////////////////////// ETAPE 2
	// Transmission du fichier PTH : PTH.TXT
	Serial.println("*[%2%]*");
	//

	dataFile = SD.open(PTH_FILENAME);

	if (dataFile) {
		while (dataFile.available())
			Serial.write(dataFile.read());
		dataFile.close();
	}

	else {
		Serial.println("*-*-*-ERROR:1-*-*-*");
	}


	///////////////////////////////////////////// ETAPE 1
	// Transmission du fichier image 1 : IMG01.JPG
	Serial.println("*[%1%]*");
	//

	dataFile = SD.open(IMG_FILENAME);

	if (dataFile) {
		while (dataFile.available())
			Serial.write(dataFile.read());
		dataFile.close();
	}

	else {
		Serial.println("*-*-*-ERROR:0-*-*-*");
	}
}

void loop(){

	///////////////////////////////////////////// ETAPE 0
	// Fin de transmission
	Serial.println("*[%0%]*");
	//

	delay(10000);


}


void start_counter() {
  start_missions = micros();
  start_flag = true;
  detachInterrupt(INTERRUPT_PIN);
}


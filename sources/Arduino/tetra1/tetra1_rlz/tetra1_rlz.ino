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
#define POS_FILENAME "POS.TXT"


//SoftwareSerial XBee(XBEE_RX,XBEE_TX);
SoftwareSerial InterArduino(7, 6); // RX, TX

File PTH_File, Cam_File, dataFile, saveFile;

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

	pinMode(5, OUTPUT);
	pinMode(4, INPUT);
	digitalWrite(5, LOW);

	//XBee.begin(9600);
	//XBee.println("Debut");

	// SD Initialisation
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT)) {
		Serial.println("SD initialization failed!");
		while(true); // ça sert à rien de continuer
	}

	//BMP085
	Wire.begin();
	bmp085Calibration();

	//RHT03
	dht.begin();


	///////////////////////////////////////////// ETAPE 10
	// L'Éveil du τετραφάρμακος
	Serial.println("*[%10%]*");
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

	///////////////////////////////////////////// ETAPE 9
	// Attente de largage ...
	Serial.println("*[%9%]*");
	//

	while(!start_flag);

	///////////////////////////////////////////// ETAPE 8
	// Largage, le cavalier saute!
	Serial.println("*[%8%]*");
	//



	///////////////////////////////////////////// ETAPE 7
	// Capture d'image (cf. étape 3 pour l'enregistrement)
	Serial.println("*[%7%]*");
	//

	SendTakePhotoCmd();

	///////////////////////////////////////////// ETAPE 6
	// Mesure PTH et enregistrement sur carte µSD
	Serial.println("*[%6%]*");
	//

	if(SD.exists(PTH_FILENAME))
		SD.remove(PTH_FILENAME);
	Serial.println(millis());
	PTH_File = SD.open(PTH_FILENAME, FILE_WRITE);
	Serial.println(millis());

	while ((micros()-start_missions) < ((unsigned long) 10000000)) {//stop_missions_time) {
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

		Serial.println(millis());

		PTH_File.println(micros()-start_missions);
	}
	PTH_File.close();

	///////////////////////////////////////////// ETAPE 5
	// Enregistrement du fichier image 1 (IMG01.JPG) sur la carte µSD
	Serial.println("*[%5%]*");
	//

	if(SD.exists(IMG_FILENAME))
		SD.remove(IMG_FILENAME);
	Cam_File = SD.open(IMG_FILENAME, FILE_WRITE);

	SaveToFile(Cam_File);

	Cam_File.close();
	ShutDownCmd();

	///////////////////////////////////////////// ETAPE 4
	// Transmission du fichier PTH : PTH.TXT
	Serial.println("*[%4%]*");
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


	///////////////////////////////////////////// ETAPE 3
	// Transmission du fichier image 1 : IMG01.JPG
	Serial.println("*[%3%]*");
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

	///////////////////////////////////////////// ETAPE 2
	// Reception du fichier : POS.TXT de la part de l'Arduino n°2
	Serial.println("*[%2%]*");
	//


	InterArduino.begin(4800);

	if(SD.exists(POS_FILENAME))
		SD.remove(POS_FILENAME);

	File saveFile = SD.open(POS_FILENAME, FILE_WRITE);

	digitalWrite(5, HIGH);

	if (saveFile) {
		while (digitalRead(4) == HIGH) {
			if (InterArduino.available())
				saveFile.write(InterArduino.read());
		}
		saveFile.close();
	}
	else
		Serial.println("*-*-*-ERROR:2-*-*-*");

	digitalWrite(5, LOW);


	///////////////////////////////////////////// ETAPE 1
	// Transmission du fichier : POS.TXT
	Serial.println("*[%1%]*");
	//

	dataFile = SD.open(POS_FILENAME);

	if (dataFile) {
		while (dataFile.available())
			Serial.write(dataFile.read());
		dataFile.close();
	}

	else
		Serial.println("*-*-*-ERROR:1-*-*-*");

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

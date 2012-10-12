#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "tetrapharmakos.h"

#include "LinkSprite.h"
//#include "BMP085.h" ////////// À décommenter pour activer le BMP

#define INTERRUPT_PIN 1
#define PTH_FILENAME "PTH.TXT"
#define IMG_FILENAME "IMG_1.JPG"
#define TEST_FILENAME "IMG04.JPG"

#define SAVE_FILENAME "IMG_2.JPG"

#define INTERRUPT_PIN 1


SoftwareSerial XBee(8,9);
SoftwareSerial InterArduino(7,6);

File PTH_File, Cam_File, dataFile;


// interrupt
volatile unsigned long start_missions = 0;
volatile boolean start_flag = false;
int count_snap=7;
int count=0;

void setup(){
	// Communication
	Serial.begin(38400);
	CamStart();

	pinMode(5, OUTPUT);
	pinMode(4, INPUT);
	digitalWrite(5, LOW);


	XBee.begin(9600);
	XBee.println("Debut");

	// SD Initialisation
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT)) {
		XBee.println("initialization failed!");
		while(true); // ça sert à rien de continuer
	}


	SendTakePhotoCmd();

	if(SD.exists(IMG_FILENAME))
		SD.remove(IMG_FILENAME);
	Cam_File = SD.open(IMG_FILENAME, FILE_WRITE);

	//SaveToFile(Cam_File);

	Cam_File.close();
	ShutDownCmd();

	XBee.println("Fin de la prise de vue.");




	if(SD.exists(SAVE_FILENAME)) {
		XBee.println("rm");
		SD.remove(SAVE_FILENAME);
	}

	File saveFile = SD.open(SAVE_FILENAME, FILE_WRITE);

	XBee.end();
	InterArduino.begin(300);
	digitalWrite(5, HIGH);
	while(digitalRead(4) == LOW) {
		XBee.print("W");
		delay(10);
	}

	if (saveFile) {
		while (digitalRead(4) == HIGH) {
		//XBee.print("H");
			if (InterArduino.available()) {
				saveFile.write(InterArduino.read());
				//XBee.print("R");
			}
		}
		saveFile.close();
	}
	else
		XBee.println("Echec d'ouverture de SAVE.JPG");


	digitalWrite(5, LOW);

	InterArduino.end();
	XBee.begin(9600);
	///////////////////////////////////////////// ETAPE 2
	// Transmission du fichier image 2 : IMG_2.JPG
	XBee.println("*[%2%]*");
	//

	dataFile = SD.open(SAVE_FILENAME);

	if (dataFile) {
		while (dataFile.available())
			XBee.write(dataFile.read());
		dataFile.close();
	}

	else {
		XBee.println("*-*-*-ERROR:0-*-*-*");
	}


	delay(1000);
	///////////////////////////////////////////// ETAPE 1
	// Transmission du fichier image 1 : IMG_1.JPG
	XBee.println("*[%1%]*");
	//

	dataFile = SD.open(TEST_FILENAME);

	if (dataFile) {
		while (dataFile.available())
			//XBee.write(dataFile.read());
		dataFile.close();
	}

	else {
		XBee.println("*-*-*-ERROR:0-*-*-*");
	}
}

void loop(){

	///////////////////////////////////////////// ETAPE 0
	// Fin de transmission
	XBee.println("*[%0%]*");
	//

	delay(10000);


}


void start_counter() {
  start_missions = micros();
  start_flag = true;
  detachInterrupt(INTERRUPT_PIN);
}

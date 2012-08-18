/*
Recopiage du programme "DumpFile.ino" donné en exemple de la bibliothèque SD.h
*/


#include <SD.h>
#include "tetrapharmakos.h"

void setup() {
	Serial.begin(9600);

	///////////////////////////////////////////// ETAPE 3
	Serial.print("*[%3%]*");

	Serial.print("Initializing SD card...");
	pinMode(10, OUTPUT);

	///////////////////////////////////////////// ETAPE 2
	Serial.print("*[%2%]*");
	if (!SD.begin(CHIP_SELECT)) {
		Serial.println("Card failed, or not present");
		return;
	}
	Serial.println("card initialized.");

	///////////////////////////////////////////// ETAPE 1
	Serial.print("*[%1%]*");
	File dataFile = SD.open("IMG01.JPG");

	if (dataFile) {
		while (dataFile.available())
			Serial.write(dataFile.read());
		dataFile.close();
	}

	else {
		Serial.println("error opening file to send");
	}
}

void loop(){
	///////////////////////////////////////////// ETAPE 0
	Serial.print("*[%0%]*");
	delay(10000);
}


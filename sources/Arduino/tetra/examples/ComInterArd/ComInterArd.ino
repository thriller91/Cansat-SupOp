/*
Programme de test de communication entre deux Arduino via une connection en SoftwareSerial.
Il y a 4 fils connectés entre les 2 Arduinos.

Il faut embarquer le programme ComInterArd2.ino sur l'autre Arduino.

Le résultat n'est pas terrible (erreurs fréquentes et lenteur). Il faut sûrement lire des packets d'octets avant de les envoyer.
*/

#include <SD.h>
#include <SoftwareSerial.h>

#define CHIP_SELECT 10

SoftwareSerial InterArduino(6, 7); // RX, TX

void setup() {
	Serial.begin(9600);
	Serial.println("Debut");

	InterArduino.begin(9600);
	pinMode(4, OUTPUT);
	pinMode(5, INPUT);
	digitalWrite(4, LOW);

	Serial.print("Initializing SD card...");
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT)) {
		Serial.println("Card failed, or not present");
		return;
	}
	Serial.println("card initialized.");

	File dataFile = SD.open("IMG01.JPG");

	delay(5000);
	digitalWrite(4, HIGH);
	if (dataFile) {
		while (dataFile.available())
			InterArduino.write(dataFile.read());
	}

	else {
		Serial.println("error opening file to send");
	}

	dataFile.close();
	digitalWrite(4, LOW);
	while(digitalRead(5) == HIGH) {
	}

}

void loop() {
	Serial.println("Fin");
	delay(1000);
}

/*
Programme de test de communication entre deux Arduino via une connection en SoftwareSerial.
Il y a 4 fils connectés entre les 2 Arduinos.

Il faut embarquer le programme ComInterArd.ino sur l'autre Arduino.

Le résultat n'est pas terrible (erreurs fréquentes et lenteur). Il faut sûrement lire des packets d'octets avant de les envoyer.
On peut aussi essayer en diminuant le Baudrate de la connection SoftwareSerial.
*/

#include <SD.h>
#include <SoftwareSerial.h>

#define CHIP_SELECT 10

#define FILENAME "X_IMG.JPG"

SoftwareSerial InterArduino(7, 6); // RX, TX


void setup() {
	Serial.begin(9600);
	Serial.println("Debut");

	Serial.print("Initializing SD card...");
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT)) {
		Serial.println("Card failed, or not present");
		return;
	}
	Serial.println("card initialized.");

	InterArduino.begin(4800);
	pinMode(5, OUTPUT);
	pinMode(4, INPUT);
	digitalWrite(5, LOW);

	if(SD.exists(FILENAME)) {
		Serial.println("rm");
		SD.remove(FILENAME);
	}

	File saveFile = SD.open(FILENAME, FILE_WRITE);

	while(digitalRead(4) == LOW) {
	}
	digitalWrite(5, HIGH);

	if (saveFile) {
		while (digitalRead(4) == HIGH) {
			if (InterArduino.available()) {
				saveFile.write(InterArduino.read());
			}
		}
		saveFile.close();
	}
	else
		Serial.println("Echec d'ouverture de SAVE.JPG");

	delay(2000);
	digitalWrite(5, LOW);
}

void loop() {
	Serial.println("Fin");
	delay(5000);
}

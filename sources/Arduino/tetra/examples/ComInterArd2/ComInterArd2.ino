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

	if(SD.exists("SAVE.JPG")) {
		Serial.println("rm");
		SD.remove("SAVE.JPG");
	}

	File saveFile = SD.open("SAVE.JPG", FILE_WRITE);

	while(digitalRead(4) == LOW) {
	}
	digitalWrite(5, HIGH);

	if (saveFile) {
		while (digitalRead(4) == HIGH) {
			if (InterArduino.available())
				saveFile.write(InterArduino.read());
		}
		saveFile.close();
	}
	else
		Serial.println("Echec d'ouverture de SAVE.JPG");

  // re-open the file for reading:
  saveFile = SD.open("SAVE.JPG");
  if (saveFile) {
    Serial.println("SAVE.JPG:");
    // read from the file until there's nothing else in it:
    while (saveFile.available()) {
        Serial.write(saveFile.read());
    }
    // close the file:
	delay(1000);
	Serial.write(saveFile.size());
    saveFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening SAVE.JPG for reading");
  }
	delay(2000);
	digitalWrite(5, LOW);
}

void loop() {
	Serial.println("Fin");
	delay(5000);
}

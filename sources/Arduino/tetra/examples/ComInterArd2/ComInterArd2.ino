/*
Programme de test de communication entre deux Arduino via une connection en SoftwareSerial.
Il y a 4 fils connectés entre les 2 Arduinos.

Il faut embarquer le programme ComInterArd.ino sur l'autre Arduino.

Le résultat n'est pas terrible (erreurs fréquentes et lenteur). Il faut sûrement lire des packets d'octets avant de les envoyer.
*/


#include <SoftwareSerial.h>

SoftwareSerial InterArduino(7, 6); // RX, TX

void setup() {
	Serial.begin(9600);
	Serial.println("Debut");

	InterArduino.begin(9600);
	InterArduino.println("Debut Soft");
}

void loop() {
	InterArduino.println("Arduino2");
	if (InterArduino.available())
		Serial.write(InterArduino.read());
	delay(1000);
}

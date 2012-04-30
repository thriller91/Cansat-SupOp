#Projet τετραφάρμακος

#à propos
Nous construisons un cansat, ce satellite, qui sera largué d'un dirigeable du CNES, doit tenir dans une canette. Il doit réaliser 
plusieurs missions. Nous allons faire une mesure de données météorologiques, et une mesure de l'indice de végétation par une prise 
d'image dans deux gammes spectrales.

#les composants

Nous utilisons une Arduino mini pour contrôler nos capteurs (pression, température, hydrométrie, accélération, position GPS), nos 
caméras (caméra JPEG), notre module de transmission sans fil (Xbee) et notre carte mémoire micro-SD.
L'ensemble de ces composants doit être mis en commun, et il est important de bien respecter les tensions de chacun. Les programmes 
permettant de piloter ces composants sont rassemblés dans ce repository.

----------------

/*
Programme principal a embarquer dans le Cansat

(remplacer les bouts de code par des fonction internes
pour plus de clareté et simplicité)
*/

#include <Wire.h>
#include <BMP085.h>
#include <DHT22.h>

//BMP085
BMP085 captTP;

//DH22
DHT22 captHT(9);

//ADXL335
const int groundpin = 18;             // analog input pin 4 -- ground
const int powerpin = 19;              // analog input pin 5 -- voltage
const int xpin = A3;                  // x-axis of the accelerometer
const int ypin = A2;                  // y-axis
const int zpin = A1;                  // z-axis (only on 3-axis models)

//GPS

void setup(){
  Serial.begin(115200);
	captTP.begin();

	// Provide ground and power by using the analog inputs as normal
	// digital pins.  This makes it possible to directly connect the
	// breakout board to the Arduino.  If you use the normal 5V and
	// GND pins on the Arduino, you can remove these lines.
	pinMode(groundpin, OUTPUT);
	pinMode(powerpin, OUTPUT);
	digitalWrite(groundpin, LOW); 
	digitalWrite(powerpin, HIGH);
}

void loop(){

	/*
	Sortie série de la pression et de la température du capteur BMP085
	*/
	Serial.print(captTP.getTemperature());
	Serial.print("  ");
	Serial.println(captTP.getPressure());
	
	/*
	Sortie série de l'humidité et de la température du capteur DH22
	*/
	Serial.print(captHT.getHumidity());
	Serial.print("  ");
	Serial.println(captHT.getTemperatureC());
	
	/*
	Sortie série des accélérations du capteur ADXL335
	*/
	Serial.print(analogRead(xpin));
	Serial.print("\t");
	Serial.print(analogRead(ypin));
	Serial.print("\t");
	Serial.println(analogRead(zpin));

}
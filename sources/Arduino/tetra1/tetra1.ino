/*
Programme à embarquer sur l'Arduino Mini n°1 du τετραφάρμακος.
TODO
-ajouter la LinkSprite n°1;
-ajouter le support de la carte SD;
-ajouter la commutation du module XBee;
-gérer la communication avec l'Arduino Mini n°2.
*/

#include <Wire.h>
#include "BMP085.h"
#include "DHT.h"

//RHT03
#define DHTPIN 2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);


void setup(){
  Serial.begin(115200);

  //BMP085
  Serial.println("BMP085");
  Wire.begin();
  bmp085Calibration();

  //RHT03
  Serial.println("RHT03");
  dht.begin();
}

void loop(){

  /*
  Sortie série de la pression et de la température du capteur BMP085
  */
  float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
  float pressure = bmp085GetPressure(bmp085ReadUP());
  Serial.print("BMP085: \t T: ");
  Serial.print(temperature, 2); //display 2 decimal places
  Serial.print(" deg C\t");
  Serial.print("P: ");
  Serial.print(pressure, 0); //whole number only.
  Serial.println(" Pa");

  /*
  Sortie série de l'humidité et de la température du capteur DH22
  */
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) {
    Serial.println("Failed to read from DHT");
  }
  else {
    Serial.print("RHT03: \t Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C");
  }
}

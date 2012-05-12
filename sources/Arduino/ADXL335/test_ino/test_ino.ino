/*
  Lecture des accélérations analogiques
 */

#include "ADXL335.h"

#define ADXL335_X_PIN 0
#define ADXL335_Y_PIN 1
#define ADXL335_Z_PIN 2
int a_x, a_y, a_z;
// Setup a ADXL335 instance
ADXL335 myADXL335(ADXL335_X_PIN,ADXL335_Y_PIN,ADXL335_Z_PIN);

void setup() {                
  Serial.begin(9600);
  Serial.println("***Accéléromètre***");
}

void loop() {
 myADXL335.read(&a_x,&a_y,&a_z);
 Serial.print(a_x);
 Serial.print(";");
 Serial.print(a_y);
 Serial.print(";");
 Serial.println(a_z);
 delay(1000); 
}

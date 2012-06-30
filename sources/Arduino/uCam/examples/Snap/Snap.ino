/*
Programme de capture d'image et de transfet via software-serial
*/

#include <SoftwareSerial.h>
#include <uCam.h>

uCam camera(0); // on utlise la camera en Serial, et on lira les images en software serial

void setup(){
	camera.begin(9600);
	camera.init(0x07,0x05); // image en couleurs JPEG avec une resolution 320x240
}

void loop(){
	camera.init(0x07,0x05); // image en couleurs JPEG avec une resolution 320x240
}

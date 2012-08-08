#include <SD.h>
#include "LinkSprite.h"

byte incomingbyte;
byte a[32];                            //Array to store image chuncks to be read off the camera
int x=0x0000,j=0,k=0;                  //Read Starting address   
uint8_t MH=0x00,ML=0x00;
boolean EndFlag=0;
int chipSelect = 10;                    //Required when using Sparkfun SD shield
File logfile;



///////////////////PUBLIC////////////////////////


// Constructor

LinkSprite::LinkSprite() {
	x=0x0000;
	x-=0x20;
	SendResetCmd();
}

// Destructor
LinkSprite::~LinkSprite() {

}

void LinkSprite::Snap() {
	SendTakePhotoCmd();
	while(Serial.available()>0) {
		incomingbyte=Serial.read();
	}
}

void LinkSprite::Save(File logfile) {
	EndFlag=0;
	while(!EndFlag) {
		j=0;
		k=0;

		SendReadDataCmd();
		delay(15);

		while(Serial.available()>0) {
			incomingbyte=Serial.read();
			k++;
			if((k>5)&&(j<32)&&(!EndFlag)) {
				a[j]=incomingbyte;
				if((a[j-1]==0xFF)&&(a[j]==0xD9))      //Check if the picture is over
					EndFlag=1;
				logfile.print((char)incomingbyte);
				j++;
			}
		}
	}
	logfile.close();
}


///////////////////PRIVATE///////////////////////


void LinkSprite::SendResetCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x26);
	Serial.write(byte(0x00));
}

void LinkSprite::ReadImageSizeCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x34);
	Serial.write(0x01);
	Serial.write(byte(0x00));
}

void LinkSprite::SetImageSizeCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x31);
	Serial.write(0x05);
	Serial.write(0x04);
	Serial.write(0x01);
	Serial.write(byte(0x00));
	Serial.write(0x19);
	Serial.write(0x22);
}

void LinkSprite::SetBaudRateCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x24);
	Serial.write(0x03);
	Serial.write(0x01);
	Serial.write(0xAE);
	Serial.write(0xC8);

}

void LinkSprite::SendTakePhotoCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x36);
	Serial.write(0x01);
	Serial.write(byte(0x00));
}

void LinkSprite::SendReadDataCmd() {
	MH=x/0x100;
	ML=x%0x100;
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x32);
	Serial.write(0x0c);
	Serial.write(byte(0x00));
	Serial.write(0x0a);
	Serial.write(byte(0x00));
	Serial.write(byte(0x00));
	Serial.write(MH);
	Serial.write(ML);
	Serial.write(byte(0x00));
	Serial.write(byte(0x00));
	Serial.write(byte(0x00));
	Serial.write(0x20);
	Serial.write(byte(0x00));
	Serial.write(0x0a);
	x+=0x20;                            //address increases 32£¬set according to buffer size
}

void LinkSprite::StopTakePhotoCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x36);
	Serial.write(0x01);
	Serial.write(0x03);
}


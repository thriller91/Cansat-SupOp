#include <SD.h>
#include <SoftwareSerial.h>
#include "LinkSprite.h"


// LinkSprite
byte incomingbyte;
bool EndFlag=0;
byte a[32];
int j=0,k=0;


int x=0x0000;                 //Read Starting address
uint8_t MH=0x00,ML=0x00;

void CamStart() {
	x=0x0000;
	x-=0x20;
	EndFlag=0;

	SetImageSizeCmd();
	SendResetCmd();
	delay(3000);
}

void SaveToFile(File f) {
	while(Serial.available()>0)	{
		incomingbyte=Serial.read();
	}

	while(!EndFlag)	{
		j=0;
		k=0;

		SendReadDataCmd();
		delay(15);

		while(Serial.available()>0)	{
			incomingbyte=Serial.read();
			k++;
			if((k>5)&&(j<32)&&(!EndFlag)) {
				a[j]=incomingbyte;
				if((a[j-1]==0xFF)&&(a[j]==0xD9))      //Check if the picture is over
					EndFlag=1;
				f.print((char)incomingbyte);
				j++;
			}
		}
	}
}

void ShutDownCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x3E);
	Serial.write(0x03);
	Serial.write(byte(0x00));
	Serial.write(0x01);
	Serial.write(0x01);
}

/*
The two following functions are not factorised
*/
File OpenNewFile() {
	char filename[] = "IMG00.jpg";
	for (uint8_t i = 0; i < 100; i++) {
		filename[3] = i/10 + '0';
		filename[4] = i%10 + '0';
		if (!SD.exists(filename)) {
			return (SD.open(filename, FILE_WRITE));
			break;
		}
	}
}

File OpenLastFile() {
	char filename[] = "IMG00.jpg";
	for (uint8_t i = 0; i < 100; i++) {
		filename[3] = i/10 + '0';
		filename[4] = i%10 + '0';
		if (!SD.exists(filename)) {
			filename[3] = (i-1)/10 + '0';
			filename[4] = (i-1)%10 + '0';
			return (SD.open(filename, FILE_WRITE));
			break;
		}
	}
}

void SendResetCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x26);
	Serial.write(byte(0x00));
}

void ReadImageSizeCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x34);
	Serial.write(0x01);
	Serial.write(byte(0x00));
}

void SetImageSizeCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	//Serial.write(0x31);
	//Serial.write(0x05);
	//Serial.write(0x04);
	//Serial.write(0x01);
	//Serial.write(byte(0x00));
	//Serial.write(0x19);
	Serial.write(0x54);
	Serial.write(0x01);
	Serial.write(byte(0x00)); // 640x480px
}

void SetBaudRateCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x24);
	Serial.write(0x03);
	Serial.write(0x01);
	Serial.write(0xAE);
	Serial.write(0xC8);

}

void SendTakePhotoCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x36);
	Serial.write(0x01);
	Serial.write(byte(0x00));

	delay(10);
	while(Serial.available()>0)
			Serial.read();
}

void SendReadDataCmd() {
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

void StopTakePhotoCmd() {
	Serial.write(0x56);
	Serial.write(byte(0x00));
	Serial.write(0x36);
	Serial.write(0x01);
	Serial.write(0x03);
}


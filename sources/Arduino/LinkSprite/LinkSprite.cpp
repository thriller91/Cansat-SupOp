#include <SD.h>
#include <SoftwareSerial.h>
#include "LinkSprite.h"


// LinkSprite
byte incomingbyte;
bool EndFlag=0;
byte a[32];
int j=0,k=0;

SoftwareSerial Seriol(8,9);


int x=0x0000;                 //Read Starting address
uint8_t MH=0x00,ML=0x00;

void CamStart() {
	x=0x0000;
	x-=0x20;
	EndFlag=0;

	Seriol.begin(38400);
	SetImageSizeCmd();
	SendResetCmd();
	delay(3000);

	SetBaudRateCmd();
	Seriol.end();
	delay(10);
	Seriol.begin(9600);
}

void SaveToFile(File f) {
	while(Seriol.available()>0)	{
		incomingbyte=Seriol.read();
	}

	while(!EndFlag)	{
		j=0;
		k=0;

		SendReadDataCmd();
		delay(15);

		while(Seriol.available()>0)	{
			incomingbyte=Seriol.read();
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
	Seriol.write(0x56);
	Seriol.write(byte(0x00));
	Seriol.write(0x3E);
	Seriol.write(0x03);
	Seriol.write(byte(0x00));
	Seriol.write(0x01);
	Seriol.write(0x01);
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
	Seriol.write(0x56);
	Seriol.write(byte(0x00));
	Seriol.write(0x26);
	Seriol.write(byte(0x00));
}

void ReadImageSizeCmd() {
	Seriol.write(0x56);
	Seriol.write(byte(0x00));
	Seriol.write(0x34);
	Seriol.write(0x01);
	Seriol.write(byte(0x00));
}

void SetImageSizeCmd() {
	Seriol.write(0x56);
	Seriol.write(byte(0x00));
	//Seriol.write(0x31);
	//Seriol.write(0x05);
	//Seriol.write(0x04);
	//Seriol.write(0x01);
	//Seriol.write(byte(0x00));
	//Seriol.write(0x19);
	Seriol.write(0x54);
	Seriol.write(0x01);
	Seriol.write(byte(0x00)); // 640x480px
}

void SetBaudRateCmd() {
	Seriol.write(0x56);
	Seriol.write(byte(0x00));
	Seriol.write(0x24);
	Seriol.write(0x03);
	Seriol.write(0x01);
	Seriol.write(0xAE);
	Seriol.write(0xC8);

}

void SendTakePhotoCmd() {
	Seriol.write(0x56);
	Seriol.write(byte(0x00));
	Seriol.write(0x36);
	Seriol.write(0x01);
	Seriol.write(byte(0x00));

	delay(10);
	while(Seriol.available()>0)
			Seriol.read();
}

void SendReadDataCmd() {
	MH=x/0x100;
	ML=x%0x100;
	Seriol.write(0x56);
	Seriol.write(byte(0x00));
	Seriol.write(0x32);
	Seriol.write(0x0c);
	Seriol.write(byte(0x00));
	Seriol.write(0x0a);
	Seriol.write(byte(0x00));
	Seriol.write(byte(0x00));
	Seriol.write(MH);
	Seriol.write(ML);
	Seriol.write(byte(0x00));
	Seriol.write(byte(0x00));
	Seriol.write(byte(0x00));
	Seriol.write(0x20);
	Seriol.write(byte(0x00));
	Seriol.write(0x0a);
	x+=0x20;                            //address increases 32£¬set according to buffer size
}

void StopTakePhotoCmd() {
	Seriol.write(0x56);
	Seriol.write(byte(0x00));
	Seriol.write(0x36);
	Seriol.write(0x01);
	Seriol.write(0x03);
}


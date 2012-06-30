/*
Programme de capture d'image et de transfet via software-serial, traduit du programme C
*/

#include <SoftwareSerial.h>

SoftwareSerial mySerial(7,8);

void setup(){
	Serial.begin(115200);
	mySerial.begin(4800);
	mySerial.print("debut");
	unsigned char response[6];
	unsigned char c;
	unsigned char r; // peut-etre un short
	unsigned char headerimgpkg[4];
	unsigned int pkgSizeIm;
	unsigned int pkgId;

	int k;

	char * pEnd;
	uint8_t ims;

	//Command identifiers bytes
	unsigned char ID0=0xAA;
	unsigned char ID_INITIAL=0x01;
	unsigned char ID_GET_PICTURE=0x04;
	unsigned char ID_SNAPSHOT=0x05;
	unsigned char ID_SET_PACKAGE_SIZE=0x06;
	unsigned char ID_SetBaudRate=0x07;
	unsigned char ID_RESET=0x08;
	unsigned char ID_DATA=0x0A;
	unsigned char ID_SYNC=0x0D;
	unsigned char ID_ACK=0x0E;
	unsigned char ID_NAK=0x0F;
	unsigned char ID_LIGHT=0x13;

	//commands
	unsigned char sync[6]={ID0,ID_SYNC,0x00,0x00,0x00,0x00};
	unsigned char init[6]={ID0, ID_INITIAL,0x00, 0x07, 0x09, 0x05};
	unsigned char ACK[6]={ID0,ID_ACK,0x00,0x00,0x00,0x00};
	unsigned char snapshot[6]={ID0,ID_SNAPSHOT,0x00,0x00,0x00,0x00};
	unsigned char setpkgsize[6]={ID0,ID_SET_PACKAGE_SIZE,0x08,0xC8,0x00,0x00};
	unsigned char getpicture[6]={ID0,ID_GET_PICTURE,0x01,0x00,0x00,0x00};
	unsigned char reset[6]={ID0,ID_RESET,0x00,0x00,0x00,0xFF};


	// Syncronisation
	mySerial.println("Sync");
	Serial.write((char*)sync);

	for (k=0;k<12;k++)	//recovers serial bytes from the camera. Does not check it, simply empties buffer and waits for all the bytes to be received.
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}
	Serial.write((char*)ACK);

	delay(1000);

	for (k=0;k<6;k++)	//recovers serial bytes from the camera. Does not check it, simply empties buffer and waits for all the bytes to be received.
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}

	delay(2000);

	mySerial.println("Snap");
	Serial.write((char*)snapshot);
	for (k=0;k<6;k++)
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}

	Serial.write((char*)setpkgsize);
	for (k=0;k<6;k++)
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}

	Serial.write((char*)getpicture);
	for (k=0;k<12;k++)
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}

	unsigned int imgSize=0;
	for (k=0;k<3;k++)	//reads package size thanks to the data package
	{
		ims=response[k+3];
		imgSize+=ims<<(8*k);
	}

	unsigned int nbpkg = ceil(static_cast<float>(imgSize)/194);	//computes number of package to receive

	delay(1000);

	ACK[2] = ID_DATA;	//sets ACK package to ask image package to the camera.

	for (k=0;k<nbpkg;k++)
	{
		ACK[4]=k;//(k&0xFF);	//id of the package to recover. Here we assume there i sless than 255 packages.
		r=ACK[4];
		//cout<<r<<endl;
		//ACK[5]=(k>>8)&0xFF;
		Serial.write((char*)ACK);
		for (int i=0;i<4;i++)	//saves the 4 first bytes of the package which contain the id and the image data size
		{
			c=Serial.read();
			r=c;
			headerimgpkg[i]=c;
			//cout<<hex<<r<<" ";
		}

		ims = headerimgpkg[0];
		pkgId = ims;
		ims = headerimgpkg[1];
		pkgId += ims<<8;
		ims = headerimgpkg[2];
		pkgSizeIm = ims;
		ims = headerimgpkg[3];
		pkgSizeIm += ims<<8;

		//cout<<dec<<"	"<<pkgId<<"	"<<pkgSizeIm<<endl;
		for (int i=0;i<pkgSizeIm;i++)	//reads the image data bytes and sends it to the standard output.
		{
			c=Serial.read();
			r=c;
			//cout<<hex<<r<<" ";
			mySerial.print(c);
		}
		mySerial.println("Fin du packet");
		c=Serial.read();
		c=Serial.read();
	}

	Serial.write((char*)reset);
}

void loop(){
	mySerial.println("fin");
	delay(10);
}


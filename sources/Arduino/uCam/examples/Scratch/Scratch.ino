/*
Programme de capture d'image et de transfet via software-serial, traduit du programme C
*/

#include <SoftwareSerial.h>

//Command identifiers bytes
#define ID0 0xAA
#define ID_INITIAL 0x01
#define ID_GET_PICTURE 0x04
#define ID_SNAPSHOT 0x05
#define ID_SET_PACKAGE_SIZE 0x06
#define ID_RESET 0x08
#define ID_DATA 0x0A
#define ID_SYNC 0x0D
#define ID_ACK 0x0E
#define ID_NAK 0x0F
#define ID_LIGHT 0x13



SoftwareSerial mySerial(7,8);

void setup(){
	Serial.begin(115200);
	mySerial.begin(115200);
	mySerial.println("debut");
	uint8_t response[6];
	uint8_t c;
	uint8_t r=255;
	uint8_t headerimgpkg[4];
	unsigned int pkgSizeIm;
	unsigned int pkgId;

	int i=0;
	int k;

	char * pEnd;
	uint8_t ims;

	//commands
	uint8_t sync[6]={ID0,ID_SYNC,0x00,0x00,0x00,0x00};
	uint8_t init[6]={ID0, ID_INITIAL,0x00, 0x07, 0x09, 0x05};
	uint8_t ACK[6]={ID0,ID_ACK,0x00,0x00,0x00,0x00};
	uint8_t snapshot[6]={ID0,ID_SNAPSHOT,0x00,0x00,0x00,0x00};
	uint8_t setpkgsize[6]={ID0,ID_SET_PACKAGE_SIZE,0x08,0xC8,0x00,0x00};
	uint8_t getpicture[6]={ID0,ID_GET_PICTURE,0x01,0x00,0x00,0x00};
	uint8_t reset[6]={ID0,ID_RESET,0x00,0x00,0x00,0xFF};


	delay(3000);

	// Syncronisation
	while(r==255 && i<500)
	{
		mySerial.print("Sync" );
		Serial.write(sync,6);

		for (k=0;k<12;k++)	//recovers serial bytes from the camera. Does not check it, simply empties buffer and waits for all the bytes to be received.
		{
			c=Serial.read();
			r=c;
			mySerial.print(r,HEX);
		}
		mySerial.println(i);
		delay(100);
		i++;
	}
	Serial.write(ACK,6);

	delay(1000);

	for (k=0;k<6;k++)	//recovers serial bytes from the camera. Does not check it, simply empties buffer and waits for all the bytes to be received.
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}

	delay(2000);

	mySerial.println("Snap");
	Serial.write(snapshot,6);
	for (k=0;k<6;k++)
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}

	Serial.write(setpkgsize,6);
	for (k=0;k<6;k++)
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}

	Serial.write(getpicture,6);
	for (k=0;k<6;k++)
	{
		c=Serial.read();
		r=c;
		//mySerial.print(r);
	}

	mySerial.println("Size");
	for (k=0;k<6;k++)	//recovers data package form the camera and saves it. Dos not check if it is a valid package though.
	{
		c=Serial.read();
		response[k] = c;
		r=c;
		mySerial.print(r,HEX);
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

	mySerial.print("Image size: ");
	mySerial.print(imgSize,DEC);
	mySerial.print(" , number of package: ");
	mySerial.println(nbpkg);
	for (k=0;k<nbpkg;k++)
	{
		ACK[4]=k;//(k&0xFF);	//id of the package to recover. Here we assume there i sless than 255 packages.
		r=ACK[4];
		//cout<<r<<endl;
		//ACK[5]=(k>>8)&0xFF;
		Serial.write(ACK,6);
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

		mySerial.print("pkgId: ");
		mySerial.print(pkgId,DEC);
		mySerial.print("    ");
		mySerial.println(pkgSizeIm);
		//cout<<dec<<"	"<<pkgId<<"	"<<pkgSizeIm<<endl;
		for (int i=0;i<pkgSizeIm;i++)	//reads the image data bytes and sends it to the standard output.
		{
			c=Serial.read();
			r=c;
			//cout<<hex<<r<<" ";
			//mySerial.print(c);
		}
		//Serial.println("Fin du packet");
		c=Serial.read();
		c=Serial.read();
	}

	Serial.write(reset,6);
}

void loop(){
	mySerial.println("fin");
	delay(1000);
}


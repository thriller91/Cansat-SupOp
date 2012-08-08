/*
Programme à embarquer sur l'Arduino Mini n°1 du τετραφάρμακος.
TODO
-ajouter la LinkSprite n°1;
-gérer la communication avec l'Arduino Mini n°2.
*/

#include <Wire.h>
#include <SD.h>
#include <SoftwareSerial.h>

#include "utils.h"

//#include "LinkSprite.h"
#include "BMP085.h"
#include "DHT.h"

SoftwareSerial XBee(XBEE_RX,XBEE_TX);
// Ouverture des fichiers, pour l'Arduino n°1
OpenFiles Files(0);
int i=0;

// LinkSprite
byte incomingbyte;
byte a[32];                            //Array to store image chuncks to be read off the camera
int x=0x0000,j=0,k=0;                  //Read Starting address   
uint8_t MH=0x00,ML=0x00;
boolean EndFlag=0;


// RHT03
#define DHTPIN 2
#define DHTTYPE DHT22 
DHT dht(DHTPIN, DHTTYPE);


void setup(){
	// Communication
	Serial.begin(115200);
	XBee.begin(9600);
	Serial.println("Debut");
	XBee.println("Debut");
	pinMode(SWITCH_A1,OUTPUT);
	pinMode(SWITCH_A2,OUTPUT);
	digitalWrite(SWITCH_A1,LOW);
	digitalWrite(SWITCH_A2,LOW);

	//BMP085
	Serial.println("BMP085");
	Wire.begin();
	bmp085Calibration();

	//RHT03
	Serial.println("RHT03");
	dht.begin();

	// LinkSprite (simple prise de vue, pour tester la longueur du code)
	x=0x0000;
	x-=0x20;
	EndFlag=0;
	SendResetCmd();                  // Reset the Camera
	delay(3000);                     //After reset, wait 2-3 second to send take picture command 

	SendTakePhotoCmd(); 
	while(Serial.available()>0)
	{
		incomingbyte=Serial.read();
	}

	while(!EndFlag)
	{
		j=0;
		k=0;

		SendReadDataCmd();
		delay(15);

		while(Serial.available()>0)
		{
			incomingbyte=Serial.read();
			k++;
			if((k>5)&&(j<32)&&(!EndFlag))
			{
				a[j]=incomingbyte;
				if((a[j-1]==0xFF)&&(a[j]==0xD9))      //Check if the picture is over
					EndFlag=1;
				Files.Cam_File.print((char)incomingbyte);
				j++;
			}
		}
	}
	Files.Cam_File.close();
}

void loop(){

	while (i<20){
		/*
		Sortie série de la pression et de la température du capteur BMP085
		*/

		float temperature = bmp085GetTemperature(bmp085ReadUT()); //MUST be called first
		float pressure = bmp085GetPressure(bmp085ReadUP());
		Files.PTH_File.print("BMP085: \t T: ");
		Files.PTH_File.print(temperature, 2); //display 2 decimal places
		Files.PTH_File.print(" deg C\t");
		Files.PTH_File.print("P: ");
		Files.PTH_File.print(pressure, 0); //whole number only.
		Files.PTH_File.println(" Pa");

		/*
		Sortie série de l'humidité et de la température du capteur DH22
		*/

		float h = dht.readHumidity();
		float t = dht.readTemperature();
		// check if returns are valid, if they are NaN (not a number) then something went wrong!
		if (isnan(t) || isnan(h)) {
			Files.PTH_File.println("Failed to read from DHT");
		}
		else {
			Files.PTH_File.print("RHT03: \t Humidity: ");
			Files.PTH_File.print(h);
			Files.PTH_File.print(" %\t");
			Files.PTH_File.print("Temperature: ");
			Files.PTH_File.print(t);
			Files.PTH_File.println(" *C");
			Serial.println(k);
		}
		i++;
	}
	Files.PTH_File.close();

	XBee.println("Fin");
	Serial.println("Fin");
	delay(1000);


}



//Camera functions

//Send Reset command
void SendResetCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x26);
      Serial.write(byte(0x00));
}

// Send image size command, image size is returned
void ReadImageSizeCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x34);
      Serial.write(0x01);
      Serial.write(byte(0x00));
}

// Set image size
void SetImageSizeCmd()
{
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

//Set up the Baud Rate of the camera
void SetBaudRateCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x24);
      Serial.write(0x03);
      Serial.write(0x01);
      Serial.write(0xAE);
      Serial.write(0xC8);

}

//Send take picture command
void SendTakePhotoCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x36);
      Serial.write(0x01);
      Serial.write(byte(0x00));  
}

//Read data
void SendReadDataCmd()
{

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

//Stop taking a picture
void StopTakePhotoCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x36);
      Serial.write(0x01);
      Serial.write(0x03);        
}

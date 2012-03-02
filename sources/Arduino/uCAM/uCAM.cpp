#include <avr/pgmspace.h>
#include "uCAM.h"
#include "Arduino.h"
#include "SoftwareSerial.h"


SoftwareSerial cameraPort(7,8);

char ID0=0xAA;
char ID_INITIAL=0x01;
char ID_GET_PICTURE=0x04;
char ID_SNAPSHOT=0x05;
char ID_SET_PACKAGE_SIZE=0x06;
char ID_SetBaudRate=0x07;
char ID_RESET=0x08;
char ID_DATA=0x0A;
char ID_SYNC=0x0D;
char ID_ACK=0x0E;
char ID_NAK=0x0F;
char ID_LIGHT=0x13;

char SYNC[6]={ID0,ID_SYNC,0x00,0x00,0x00,0x00};

uCAM::uCAM(bool softSerial)
{
	_softSerial=softSerial;
	_baudrate=115200;
	_packageSize=64;
}

void uCAM::commINITIAL(char* response,char colourType,char RAWres,char JPEGres)
{
	char command[6]={ID0,ID_INITIAL,0x00,colourType,RAWres,JPEGres};
	sendCommand(command,response,6);
}

int uCAM::getPictureAndSize(char* response,char PictureType)
{
	char command[6]={ID0,ID_GET_PICTURE,PictureType,0x00,0x00,0x00};
	sendCommand(command,response,6);
	read(response,6);
	_imageSize=response[3]+(response[4]<<8)+(response[5]<<16);
	_nbPackages=ceil((float)_imageSize/(_packageSize-6));
}

void uCAM::commSNAPSHOT(char* response,char SnapshotType=0x00)
{
	char command[6]={ID0,ID_SNAPSHOT,SnapshotType,0x00,0x00,0x00};
	sendCommand(command,response,6);
}

void uCAM::commSET_PACKAGE_SIZE(char* response)
{
	char size[2]={_packageSize%0x100,_packageSize/0x100};
	char command[6]={ID0,ID_SET_PACKAGE_SIZE,0x08,size[0],size[1],0x00};
	sendCommand(command,response,6);
}

void uCAM::commSetBaudRate(char* response,char div1,char div2)
{
	char command[6]={ID0,ID_SetBaudRate,div1,div2,0x00,0x00};
	sendCommand(command,response,6);
}

void uCAM::commRESET(char* response,bool wholeSyst,bool special)
{
	char p1=0x01;
	char p4=0x00;
	if (wholeSyst) p1=0x00;
	if (special) p4=0x01;
	char command[6]={ID0,ID_RESET,p1,0x00,0x00,p4};
	sendCommand(command,response,6);
}

void uCAM::getPackage(char* package,char* packID,char counter)
{
	char command[6]={ID0,ID_ACK,0x00,counter,packID[0],packID[1]};
	sendCommand(command,package,_packageSize);
}

void uCAM::commACK(char* ACK,char id)
{
	ACK[0]=ID0;
	ACK[1]=ID_ACK;
	ACK[2]=id;
	for (int k=3;k<7;k++)
	{
		ACK[k]=0;
	}
}

void uCAM::begin(void)
{
	char response[6];
	int k=0;
	char comm[6];
	
	if (_softSerial)
	{
		cameraPort.begin(_baudrate);
	}
	else
	{
		Serial.begin(_baudrate);
	}
	
	while (k<61 & !available())
	{
		write(SYNC);
		k++;
	}
	if (available())
	{
		read(response,6);
		read(response,6);
		commACK(comm,response[2]);
		write(comm);
	}
	else
	{
		return;
	}
	commSET_PACKAGE_SIZE(response);
	
	return;
}

//Send a command to the camera
//pre: cameraPort is a serial connection to the camera set to 3800 baud
//     command is a string with the command to be sent to the camera
//     response is an empty string
//	   length is the number of characters in the command
//post: response contains the camera response to the command
//return: the number of characters in the response string
//usage: None (private function)
int uCAM::sendCommand(const char * command, char * response,int length)
{
	char c=0;
	int count=0;
	//Clear any data currently in the serial buffer
	if (_softSerial)
	{
		cameraPort.flush();
	}
	else
	{
		Serial.flush();
	}
	//Send each character in the command string to the camera through the camera serial port
	for(int i=0; i<6; i++)
	{
		if (_softSerial)
		{
			cameraPort.print(*command++);
		}
		else
		{
			Serial.print(*command++);
		}
	}
	//Get the response from the camera and add it to the response string.
	for(int i=0; i<length; i++)
	{
		if (_softSerial)
		{
			while(!cameraPort.available());
			*response++=cameraPort.read();
		}
		else
		{
			while(!Serial.available());
			*response++=Serial.read();		
		}
		count+=1;
	}
	
	//return the number of characters in the response string
	return count;
}

void uCAM::read(char* response,int length)
{
	for(int i=0; i<length; i++)
	{
		if (_softSerial)
		{
			while(!cameraPort.available());
			*response++=cameraPort.read();
		}
		else
		{
			while(!Serial.available());
			*response++=Serial.read();		
		}
	}
}

void uCAM::write(char* command)
{
	for(int i=0; i<6; i++)
	{
		if (_softSerial)
		{
			cameraPort.print(*command++);
		}
		else
		{
			Serial.print(*command++);
		}
	}
}

bool uCAM::available()
{
		if (_softSerial)
		{
			return cameraPort.available();
		}
		else
		{
			return Serial.available();	
		}
}

int uCAM::getPicture(char* data)
{
	int dataSize;
	char* package;
	package=new char[_packageSize];
	_lastPackage++;
	char packId[2]={_lastPackage%0x100,_lastPackage/0x100};
	getPackage(package,packId,0x00);
	dataSize=package[2]+(package[3]<<8);
	for (int k=0;_packageSize-6;k++)
	{
		data[k]=package[k+4];
	}
	delete package;
	return dataSize;
}


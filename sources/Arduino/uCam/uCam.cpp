/* 2012 test version
Written by Denis Savoie.

This library allows to use a 4dsystems' µCam camera whith an Arduino card.
Works whith Arduino 1.0, and currently tested on Arduino mini V4.

free to use and redistribute.
*/

#include <avr/pgmspace.h>
#include "uCam.h"
#include "Arduino.h"
#include "SoftwareSerial.h"


SoftwareSerial cameraPort(UCAM_RXSOFT,UCAM_TXSOFT);

using namespace uCam;


////////////PUBLIC///////////////////////////

//constructor.
//softserial : if true, uses a software serial port using SoftwareSerial library from Arduino.
uCam::uCam(bool softserial)
{
	_softserial = softserial;
	_command[0] = UCAM_ID0;
}

uCam::~uCam()		//destructor
{

}


//call it before using the camera. Sets serial speed for the camera, and synchronises the camera.
bool uCam::begin(long speed)		
{
	uint8_t k=0;
	
	setSnapshot();
	
	_speed=speed;
	
	if (_softserial)
	{
		cameraPort.begin(_speed);
		if (!cameraPort.isListening())	//ensures that the software serial port is listening.
		{
			cameraPort.listen();
		}
	}
	else
	{
		Serial.begin(_speed);
	}
	
	while (!available() && k<60)
	{
		sendSync();
		delay(1000);
		k++;
	}
	if (!available())
	{
		_errorStr = "No response";
		return false;
	}
	getResponse();
	if (!isACK())
	{
		_errorStr = "Not ACK";
		return false;
	}
	getResponse();
	sendACK();
	return true;
}

void uCam::end()	//closes communications whith the camera.
{
	if (_softserial)
	{
		cameraPort.end();
	}
	else
	{
		Serial.end();
	}
}


//Initializes the camera whith the following parameters :
//colortype : selects color or gray, and number of bit per pixel.
//res : selects the resolution
//Constants are defined in uCam.h to choose these parrameters.
//By default, color is JPEG and resolution is 320x240. Note that only certain resolution are available in JPEG.
//This function is to be used after synchronization.
bool uCam::init(char colorType /*= UCAM_JPEG*/,char res /*= UCAM_RES320X240*/)
{
	if (_softserial)
	{
		if (!cameraPort.isListening())
		{
			cameraPort.listen();
		}
	}
	_resolution = res;
	_colorType = colorType;
	
	setCommand(UCAM_ID_INITIAL,0x00,_colorType,_resolution,_resolution);
	
	sendCommand();
	getResponse();
	if (!isACK())
	{
		return false;
	}
	return true;
} 


//Sets the snapshot command.
//type : chooses wheither to take JPEG or RAW snapshot.
//skipframe : number of frames to skip before taking the picture.
//can be called anytime;
void uCam::setSnapshot(char type /*= UCAM_SNAPSHOT_JPEG*/,uint16_t skipFrame /*= 0*/)
{
	_snapshotType = type;
	_skipFrame[0] = skipFrame & 0xFF;
	_skipFrame[1] = skipFrame >> 8;
}


//Sends snapshot command.
//This function must be used after synchronisation, initialization and snapshot setting
bool uCam::snapshot()
{
	setCommand(UCAM_ID_SNAPSHOT,_snapshotType,_skipFrame[0],_skipFrame[1],0x00);
	sendCommand();
	getResponse();
	if (!isACK())
	{
		return false;
	}
	return true;
}

//Sets package size. 
//This function must be used after synchronisation.
bool uCam::setPkgSize(uint16_t size)
{
	char packageSizeBytes[2];
	_packageSize = size;
	packageSizeBytes[0] = _packageSize & 0xFF;
	packageSizeBytes[1] = _packageSize >> 8;
	setCommand(UCAM_ID_SET_PACKAGE_SIZE,0x08,packageSizeBytes[0],packageSizeBytes[1],0x00);
	sendCommand();
	getResponse();
	if (!isACK())
	{
		return false;
	}
	return true;
}


//sends get picture command and recovers picture size and deduces number of packets.
bool uCam::getPicture(char type /*= UCAM_GET_PICTURE_SNAPSHOT*/)
{
	setCommand(UCAM_ID_GET_PICTURE,type,0x00,0x00,0x00);
	sendCommand();
	getResponse();
	if (!isACK())
	{
		return false;
	}
	getResponse();
	_pictureLength = _response[3] + (_response[4]<<8) +(_response[5]<<16);
	_nbPkg=_pictureLength/(_packageSize-6)+1;
	return true;
}


//requests next package
void uCam::getNextPackage()
{
	char currentPkgBytes[2];
	currentPkgBytes[0] = _currentPkg & 0xFF;
	currentPkgBytes[1] = _currentPkg >> 8;
	setCommand(UCAM_ID_ACK,UCAM_ID_DATA,0x00,currentPkgBytes[0],currentPkgBytes[1]);
	sendCommand();
	_currentPkg++;
}

//gets the next serial byte from the camera.
char uCam::getDataByte()
{
	if (_softserial)
	{
		return cameraPort.read();
	}
	else
	{
		return Serial.read();
	}
}

//returns true if there is still packages to recover.
bool uCam::pkgAvailable()
{
	if (_currentPkg < _nbPkg)
	{
		return true;
	}
	return false;
}

//calls HardwareSerial::available() or SoftwareSerial::available(), and returns its response.
bool uCam::available()
{
	if (_softserial)
	{
		return cameraPort.available();
	}
	else
	{
		return Serial.available();
	}
}

String uCam::getError()	//for debug. Allows to return a string containing a message.
{
	return _errorStr;
}

void uCam::lastResponse(char * response)	//for debug. Returns last response received.
{
	for (int k = 0;k < 6;k++)
	{
		response[k]=_response[k];
	} 
}

void uCam::currentCommand(char * command)	//for debug. Returns last response that has been set.
{
	for (int k = 0;k < 6;k++)
	{
		command[k]=_command[k];
	} 
}

unsigned long uCam::imageSize()	//returns image size.
{
	return _pictureLength;
}

//String uCam::debugfunc()
//{
//	String a;
//	a = String(sizeof(char));
//	return a;
//}








///////////////////PRIVATE////////////////////////////////////


//sets syncronization command
void uCam::sendSync()
{
	setCommand(UCAM_ID_SYNC,0x00,0x00,0x00,0x00);
	sendCommand();
}

//sets aknowledgment command
void uCam::sendACK()
{
	setCommand(UCAM_ID_ACK,_response[1],0x00,0x00,0x00);
	sendCommand();
}

//returns true if the last received command from the camera is an ACK command.
bool uCam::isACK()
{
	if (_response[1]==UCAM_ID_ACK)
	{
		return true;
	}
	return false;
}

//sets a command to send to the camera
void uCam::setCommand(char p1,char p2,char p3,char p4,char p5)
{
	_command[0] = UCAM_ID0;
	_command[1] = p1;
	_command[2] = p2;
	_command[3] = p3;
	_command[4] = p4;
	_command[5] = p5;
}	

//sends the command to the camera
void uCam::sendCommand()
{
	if (_softserial)
	{
		for (int k = 0;k < 6;k++)
		{
			cameraPort.print(_command[k]);
		}
		//cameraPort.print(_command);
	}
	else
	{
		for (int k = 0;k < 6;k++)
		{
			Serial.print(_command[k]);
		}
		//Serial.print(_command);
	}
}

//gets the response from the camera.
void uCam::getResponse()
{
	unsigned long startTime=millis();
	while (!available() && (millis()-startTime < UCAM_TIMEOUT)){}
	if (_softserial)
	{
		for (int k = 0;k < 6;k++)
		{
			_response[k] = cameraPort.read();
		}
	}
	else
	{
		for (int k = 0;k < 6;k++)
		{
			_response[k] = Serial.read();
		}
	}
}



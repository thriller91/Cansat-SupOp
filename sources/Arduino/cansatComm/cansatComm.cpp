#include <avr/pgmspace.h>
#include "cansatComm.h"
#include "Arduino.h"
#include "SoftwareSerial.h"

SoftwareSerial commPort(7,6);


cansatComm::cansatComm(uint8_t id,uint8_t devices)
{
	_headerdat[0] = 0x10;
	_headerdat[1] = 0x65;
	_headerdat[2] = 0x07;
	_headerdat[3] = 0xDD;

	_headerdat[4] = 0x06;
	
	_headerdat[5] = 0x00;
	_headerdat[5] += id<<5;
	_headerdat[5] += devices & 0x1F;
	
	_headercam[0] = 0x10;
	_headercam[1] = 0x65;
	_headercam[2] = 0x07;
	_headercam[3] = 0xDD;

	_headercam[4] = 0x01;
	
	_headercam2[0] = 0x10;
	_headercam2[1] = 0x65;
	_headercam2[2] = 0x07;
	_headercam2[3] = 0xDD;

	_headercam2[4] = 0x10;	
	
	_headertext[0] = 0x10;
	_headertext[1] = 0x65;
	_headertext[2] = 0x07;
	_headertext[3] = 0xDD;
	
	_headertext[4] = 0x02;
	
	_imgid = 0x00;
	
	_devices = devices;
	
	_currentIndex=0xFF;
}

cansatComm::~cansatComm()
{
	
}


void cansatComm::begin(unsigned long speed,uint8_t nbr_row)
{
	commPort.begin(speed);
	
	
	_nbr_row=nbr_row;
}

void cansatComm::headerImgPkg()
{
	char header[3];
	header[0] = _imgid;
	for (int k=0;k<2;k++)
	{
		header[k+1] = (_pkgSize>>(8*k)) & 0xFF;
	}
	for (int k=0;k<5;k++)
	{
		commPort.print(_headercam[k]);
	}
	for (int k=0;k<3;k++)
	{
		commPort.print(header[k]);
	}
}

void cansatComm::headerImgDate()
{
	char header[9];
	header[0] = _imgid;
	for (int k=0;k<4;k++)
	{
		header[k+1] = (_lastImgDate>>(8*k)) & 0xFF;
	}
	for (int k=0;k<2;k++)
	{
		header[k+5] = (_nbrPkg>>(8*k)) & 0xFF;
	}
	for (int k=0;k<2;k++)
	{
		header[k+7] = (_pkgSize>>(8*k)) & 0xFF;
	}
	for (int k=0;k<5;k++)
	{
		commPort.print(_headercam2[k]);
	}
	for (int k=0;k<9;k++)
	{
		commPort.print(header[k]);
	}
}

void cansatComm::SendImgPkg()
{
	for (int k=0;k<_pkgSize;k++)
	{
		commPort.print(Serial.read());
	}
}

void cansatComm::new_datas()
{
	if (_currentIndex<_nbr_row-1)
	{
		_currentIndex++;
	}
	if  (_currentIndex == 0xFF)
	{
		_currentIndex = 0x00;
	}
	_date_millis[_currentIndex] = millis();
}

void cansatComm::storeBMP085(short temperature,long pressure)
{
	_tempBMP085[_currentIndex] = temperature;
	_pressure[_currentIndex] = pressure;
}

void cansatComm::storeDHT22(float temperature,float humidity)
{
	_tempDHT22[_currentIndex] = temperature;
	_humidity[_currentIndex] = humidity;
}

void cansatComm::storeAccelerometer(int16_t x,int16_t y,int16_t z)
{
	_xaxis[_currentIndex] = x;
	_yaxis[_currentIndex] = y;
	_zaxis[_currentIndex] = z;
}

void cansatComm::storeNMEAdata(String NMEAdata)
{
	_NMEAdata[_currentIndex] = NMEAdata;
}

void cansatComm::sendData()
{
	for (int k=0;k<=_currentIndex;k++)
	{
		for (int i=0;i<6;i++)
		{
			commPort.print(_headerdat[i]);
		}
		
		commPort.print("	");
		
		//date_millis
		commPort.print(_date_millis[k]);
		commPort.print("	");
	
		//BMP085
		if ((_devices & 0x08)>0)
		{
			commPort.print(_tempBMP085[k]);
			commPort.print("	");
			commPort.print(_pressure[k]);
			commPort.print("	");
		}
	
		//DHT22
		if ((_devices & 0x04)>0)
		{
			commPort.print(_tempDHT22[k]);
			commPort.print("	");
			commPort.print(_humidity[k]);
			commPort.print("	");
		}
	
		//accelerometre
		if ((_devices & 0x02)>0)
		{
			commPort.print(_xaxis[k]);
			commPort.print("	");
			commPort.print(_yaxis[k]);
			commPort.print("	");
			commPort.print(_zaxis[k]);
			commPort.print("	");
		}
	
		//GPS
		if ((_devices & 0x01)>0)
		{
			commPort.println("");
			commPort.print(_NMEAdata[k]);
		}
		commPort.println("");
		
	}
	_currentIndex=0xFF;
}


void cansatComm::sendText(char * text)
{
	for (int k=0;k<5;k++)
	{
		commPort.print(_headertext[k]);
	}
	commPort.println(text);
}

void cansatComm::sendText(long n)
{
	for (int k=0;k<5;k++)
	{
		commPort.print(_headertext[k]);
	}
	commPort.println(n);
}

void cansatComm::sendText(long n,int opt)
{
	for (int k=0;k<5;k++)
	{
		commPort.print(_headertext[k]);
	}
	commPort.println(n,opt);
}

void cansatComm::setPkgSize(uint16_t pkgSize)
{
	_pkgSize = pkgSize;
}

void cansatComm::setnbPkg(uint16_t nbrPkg)
{
	_nbrPkg = nbrPkg;
}

void cansatComm::setimgid(char imgid)
{
	_imgid=imgid;
}

void cansatComm::setImgDate()
{
	_lastImgDate = millis();
}

void cansatComm::sendImgPkg(char * imgPkg,uint16_t length)
{
	for (int k=0;k<length;k++)
	{
		commPort.print(imgPkg[k]);
	}
}


#include <avr/pgmspace.h>
#include "cansatComm.h"
#include "Arduino.h"
#include "SoftwareSerial.h"
#include "new.h"

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
	
	_imgid = 0x00;
	
	_devices = devices;
	
	_currentIndex=0xFF;
}

cansatComm::~cansatComm()
{
	//delete _date_millis, _tempBMP085, _pressure, _tempDHT22, _humidity, _xaxis, _yaxis, _zaxis, _NMEAdata;
}


void cansatComm::begin(unsigned long speed,uint8_t nbr_row)
{
	commPort.begin(speed);
	
	
	_nbr_row=nbr_row;
	
	/*//date_millis
	_date_millis = new unsigned long(nbr_row);
	
	//BMP085
	if ((_devices & 0x08)>0)
	{
		_tempBMP085 = new short(nbr_row);
		_pressure = new long(nbr_row);
	}
	
	//DHT22
	if ((_devices & 0x04)>0)
	{
		_tempDHT22 = new float(nbr_row);
		_humidity = new float(nbr_row);
	}
	
	//accelerometre
	if ((_devices & 0x02)>0)
	{
		_xaxis = new int16_t(nbr_row);
		_yaxis = new int16_t(nbr_row);
		_zaxis = new int16_t(nbr_row);
	}
	
	//GPS
	if ((_devices & 0x01)>0)
	{
		_NMEAdata = new String(nbr_row);
	}*/
}

void cansatComm::headerImgPkg()
{
	for (int k=0;k<5;k++)
	{
		commPort.print(_headercam[k]);
	}
	commPort.print(_imgid);
}

void cansatComm::headerImgDate()
{
	for (int k=0;k<5;k++)
	{
		commPort.print(_headercam[k]);
	}
	commPort.print(_lastImgDate);
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

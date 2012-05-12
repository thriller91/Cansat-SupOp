#include <avr/pgmspace.h>
#include "cansatComm.h"
#include "Arduino.h"
#include "SoftwareSerial.h"

SoftwareSerial commPort(7,6);


//constructeur
//initialise les en-têtes.
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
	
	_headerraw[0] = 0x10;
	_headerraw[1] = 0x65;
	_headerraw[2] = 0x07;
	_headerraw[3] = 0xDD;

	_headerraw[4] = 0x0A;
	
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

//destructeur
cansatComm::~cansatComm()
{
	
}

//commence la communication en utlisant un port SoftwareSerial.
//speed : vitesse de la liaison serie
//nbr_row : nombre de series de mesures maximal que peut prendre le buffer.
void cansatComm::begin(unsigned long speed,uint8_t nbr_row)
{
	commPort.begin(speed);
	
	
	_nbr_row=nbr_row;
}

//envoie un en-tête pour les paquets d'image. 
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

//envoie la taille des paquets, le nombre de paquets, la durée écoulée depuis la mise sous tension ou le dernier redemarage pour la dernière photo prise. 
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

//envoie un nombre d'octets lus sur la camera égal a _pkgSize. Peut etre utilise a envoyer un paquet d'image.
void cansatComm::SendImgPkg()
{
	for (int k=0;k<_pkgSize;k++)
	{
		while (!Serial.available()){};
		commPort.print(Serial.read());
	}
}

//A appeler juste avant de commencer une nouvelle serie d'acquisition de donnees.
//Permet de les dater par rapport au dernier redemarage, et de passer a la serie de donnees suivante en buffer
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

//Stocke les donnees acquises avec le BMP085
//temperature : temperature, short integer
//pressure : pression, long integer
void cansatComm::storeBMP085(short temperature,long pressure)
{
	_tempBMP085[_currentIndex] = temperature;
	_pressure[_currentIndex] = pressure;
}

//Stocke les donnees acquises avec le DHT22
//temperature : temperature, float
//pressure : pression, float
void cansatComm::storeDHT22(float temperature,float humidity)
{
	_tempDHT22[_currentIndex] = temperature;
	_humidity[_currentIndex] = humidity;
}

//Stocke les donnees acquises avec le DHT22
//x : valeur pour l'axe x
//y : valeur pour l'axe y
//z : valeur pour l'axe z
void cansatComm::storeAccelerometer(int16_t x,int16_t y,int16_t z)
{
	_xaxis[_currentIndex] = x;
	_yaxis[_currentIndex] = y;
	_zaxis[_currentIndex] = z;
}

//Stocke les donnees acquises avec le GPS
//NMEAdata : chaine de caractere contenant les donnees GPS sous forme de texte
void cansatComm::storeNMEAdata(String NMEAdata)
{
	_NMEAdata[_currentIndex] = NMEAdata;
}

//envoie toutes les donnees en buffer
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

//envoie une ligne de texte avec l'en-tete approprie
//text : chaine de caractere a envoyer
void cansatComm::sendText(char * text)
{
	for (int k=0;k<5;k++)
	{
		commPort.print(_headertext[k]);
	}
	commPort.println(text);
}

//envoie un nombre sous forme d'une ligne de texte avec l'en-tete approprie
//n : nombre a envoyer
void cansatComm::sendText(long n)
{
	for (int k=0;k<5;k++)
	{
		commPort.print(_headertext[k]);
	}
	commPort.println(n);
}

//envoie un nombre sous forme d'une ligne de texte avec l'en-tete approprie
//n : nombre a envoyer
//opt : parametre optionnel de SoftwareSerial::print.
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

/*
void cansatComm::sendImgPkg(char * imgPkg,uint16_t length)
{
	for (int k=0;k<length;k++)
	{
		commPort.print(imgPkg[k]);
	}
}


void cansatComm::headerRawImg()
{
	char header[6];
	header[0] = _imgid;
	
	header[1] = 0xF0;
	header[2] = 0x00;
	header[3] = 0x40;
	header[4] = 0x01;
	header[5] = 0x08;
	
	for (int k=0;k<5;k++)
	{
		commPort.print(_headerraw[k]);
	}
	for (int k=0;k<6;k++)
	{
		commPort.print(header[k]);
	}
}

*/

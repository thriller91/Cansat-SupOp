#include "commGPS.h"
#include <fstream>
#include <iostream>

using namespace std;


bool isHeader(char c)
{
	static short a = 0;
	char header[4] = {0x10,0x65,0x07,0xDD};
	
	if (c == header[a])
	{
		a++;
	}
	
	if (a == 4)
	{
		a = 0;
		return 1;
	}
	else
	{
		return 0;
	}
}

int Rnbr(uint8_t descByte)
{
	int nbr=0;
	if ((descByte & 0x01) > 0)
	{
		nbr++;
	}
	if ((descByte & 0x1F) >0 )
	{
		nbr++;
	}
	
	return nbr;	
}

bool checkPkg(char* pkg,unsigned int pkgSize)
{
	unsigned long sum=0;
	for (int k=0;k<pkgSize-2;k++)
	{
		sum+=pkg[k];
	}
	
	uint8_t sumLastBytes = sum & 0xFF;
	
	uint8_t check = pkg[pkgSize-2];
	
	if (sumLastBytes == check)
	{
		return 1;
	}
	
	return 0;
}


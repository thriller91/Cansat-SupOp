#ifndef BMP085_h
#define BMP085_h
#include "Arduino.h"
#include "Wire.h"

#define BMP085_ADDRESS 0x77  // I2C address of BMP085

class BMP085
{
	public:
		BMP085();
		void begin();
		short getTemperature();
		long getPressure();
		void bmp085Calibration();
		short bmp085GetTemperature(unsigned int ut);
		long bmp085GetPressure(unsigned long up);
		char bmp085Read(unsigned char address);
		int bmp085ReadInt(unsigned char address);
		unsigned int bmp085ReadUT();
		unsigned long bmp085ReadUP();
	private:
		int ac1;
		int ac2; 
		int ac3; 
		unsigned int ac4;
		unsigned int ac5;
		unsigned int ac6;
		int b1; 
		int b2;
		int mb;
		int mc;
		int md;
		long b5;
		unsigned char OSS;  // Oversampling Setting
};

#endif

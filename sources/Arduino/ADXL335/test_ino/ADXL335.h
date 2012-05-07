#ifndef ADXL335_h
#define ADXL335_h
#include "Arduino.h"

class ADXL335
{
	public:
		ADXL335(int xpin, int ypin, int zpin);
		void read(int *x,int *y,int *z);
	private:
		int _xpin;
		int _ypin;
		int _zpin;
};

#endif


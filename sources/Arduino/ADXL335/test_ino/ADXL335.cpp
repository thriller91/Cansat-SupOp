#include "ADXL335.h"

ADXL335::ADXL335(int xpin,int ypin,int zpin)
{
	_xpin=xpin;
	_ypin=ypin;
	_zpin=zpin;
}

void ADXL335::read(int *x, int *y, int *z)
{
	*x=analogRead(_xpin);
	*y=analogRead(_ypin);
	*z=analogRead(_zpin);
}


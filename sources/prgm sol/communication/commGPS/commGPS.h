#ifndef COMMGPS_H
#define COMMGPS_H


#include "commGPS.h"
#include <fstream>
#include <iostream>
#include <inttypes.h>


bool isHeader(char c);
int Rnbr(uint8_t descByte);
bool checkPkg(char* pkg,unsigned int pkgSize);

#endif



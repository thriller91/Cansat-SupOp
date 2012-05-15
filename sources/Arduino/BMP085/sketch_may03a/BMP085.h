#include <Wire.h>
#include <Arduino.h>

void bmp085Calibration();
float bmp085GetTemperature(unsigned int ut);
long bmp085GetPressure(unsigned long up);
char bmp085Read(unsigned char address);
int bmp085ReadInt(unsigned char address);
unsigned int bmp085ReadUT();
unsigned long bmp085ReadUP();
void writeRegister(int deviceAddress, byte address, byte val);
int readRegister(int deviceAddress, byte address);
float calcAltitude(float pressure);



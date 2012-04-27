#ifndef CANSATCOMM_h
#define CANSATCOMM_h

#include <avr/pgmspace.h>
#include "Arduino.h"
#include "new.h"
#include <inttypes.h>

class cansatComm
{
	public:
		cansatComm(uint8_t id,uint8_t devices);
		~cansatComm();
		void begin(unsigned long speed,uint8_t nbr_row);
		void headerImgPkg();
		void headerImgDate();
		void new_datas();
		void storeBMP085(short temperature,long pressure);
		void storeDHT22(float temperature,float humidity);
		void storeAccelerometer(int16_t x,int16_t y,int16_t z);
		void storeNMEAdata(String NMEAdata);
		void sendData();

		
	private:
		uint8_t _devices;
		char _headerdat[6];
		char _headercam[5];
		uint8_t _nbr_row;
		char _imgid;
		unsigned long _lastImgDate;
		unsigned long *_date_millis;
		short *_tempBMP085;
		long *_pressure;
		float *_tempDHT22,*_humidity;
		int16_t *_xaxis,*_yaxis,*_zaxis;
		String *_NMEAdata;
		uint8_t _currentIndex;
};





#endif



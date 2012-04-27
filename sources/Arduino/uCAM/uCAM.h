#ifndef uCAM_h
#define uCAM_h

#include <avr/pgmspace.h>
#include "Arduino.h"
#include <inttypes.h>

class uCAM
{
	public:
		uCAM(bool softSerial);
		void begin(void);
		int getPictureAndSize(char* response,char PictureType);
		void commSNAPSHOT(char* response,char SnapshotType);
		void commSET_PACKAGE_SIZE(char* response);
		void commSetBaudRate(char* response,char div1,char div2);
		int getPicture(char* data);
		
	private:
		bool _softSerial;
		int _baudrate;
		int _nbPackages;
		int _imageSize;
		int _lastPackage;
		int _packageSize;
		
		void commINITIAL(char* response,char colourType,char RAWres,char JPEGres);
		void commRESET(char* response,bool wholeSyst,bool special);
		void getPackage(char* package,char* packID,char counter);
		void commACK(char* ACK,char id);
		int sendCommand(const char * command, char * response,int length);
		void read(char* response,int length);
		void write(char* command);
		bool available();
};

#endif


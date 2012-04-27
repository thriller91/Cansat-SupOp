#ifndef uCAM_h
#define uCAM_h

#include <avr/pgmspace.h>
#include "Arduino.h"
#include <inttypes.h>


#define UCAM_ID0 0xAA
#define UCAM_ID_INITIAL 0x01
#define UCAM_ID_GET_PICTURE 0x04
#define UCAM_ID_SNAPSHOT 0x05
#define UCAM_ID_SET_PACKAGE_SIZE 0x06
#define UCAM_ID_SET_BAUD_RATE 0x07
#define UCAM_ID_RESET 0x08
#define UCAM_ID_DATA 0x0A
#define UCAM_ID_SYNC 0x0D
#define UCAM_ID_ACK 0x0E
#define UCAM_ID_NAK 0x0F
#define UCAM_ID_LIGHT 0x13

#define UCAM_GRAY2_RAW 0x01
#define UCAM_GRAY4_RAW 0x02
#define UCAM_GRAY8_RAW 0x03
#define UCAM_COLOR8_RAW 0x04
#define UCAM_COLOR12_RAW 0x05
#define UCAM_COLOR16_RAW 0x06
#define UCAM_JPEG 0x07

#define UCAM_RES80X60 0x01
#define UCAM_RES160X120 0x03
#define UCAM_RES320X240 0x05
#define UCAM_RES640X480 0x07
#define UCAM_RES128X128 0x09
#define UCAM_RES128X96 0x0B

#define UCAM_SNAPSHOT_JPEG 0x00
#define UCAM_SNAPSHOT_RAW 0x01

#define UCAM_GET_PICTURE_SNAPSHOT 0x01
#define UCAM_GET_PICTURE_RAW 0x02
#define UCAM_GET_PICTURE_JPEG 0x05

#define UCAM_TIMEOUT 100

#define UCAM_RXSOFT 5
#define UCAM_TXSOFT 4


class uCam
{
	public:
		uCam(bool softserial);
		~uCam();
		bool begin(long speed);
		void end();
		bool init(char colorType = UCAM_JPEG,char res = UCAM_RES320X240);
		void setSnapshot(char type = UCAM_SNAPSHOT_JPEG,uint16_t skipFrame = 0);
		bool snapshot();
		bool setPkgSize(uint16_t size);
		bool getPicture(char type = UCAM_GET_PICTURE_SNAPSHOT);
		void getNextPackage();
		char getDataByte();
		bool pkgAvailable();
		bool available();
		String getError();
		void lastResponse(char * response);
		void currentCommand(char * command);
		unsigned long imageSize();
		//String debugfunc();
		
	private:

		void sendSync();
		void sendACK();
		bool isACK();
		void setCommand(char p1,char p2,char p3,char p4,char p5);
		void sendCommand();
		void getResponse();
		
		
			
		bool _softserial;
		unsigned long _speed;
		char _resolution;
		char _colorType;
		char _snapshotType;
		char _skipFrame[2];
		uint16_t _packageSize;
		char _command[6];
		char _response[6];
		unsigned long _pictureLength;
		uint16_t _nbPkg;
		uint16_t _currentPkg;
		String _errorStr;
};



#endif



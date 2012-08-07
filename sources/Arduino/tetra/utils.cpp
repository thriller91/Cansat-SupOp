#include <Arduino.h>
#include <SD.h>
#include "utils.h"

///////////////////PUBLIC////////////////////////


// Constructor
// Choisir: 0 pour l'Arduino n°1 (le maître), 1 pour l'Arduino n°2

OpenFiles::OpenFiles(bool Arduino_num) {
	_Arduino_num = Arduino_num;
	SD.begin(CHIP_SELECT);
	OpenFile_SD();
	if (!_Arduino_num)
		OpenFile_PTH();
}

// Destructor
OpenFiles::~OpenFiles() {

}


///////////////////PRIVATE///////////////////////



void OpenFiles::OpenFile_SD() {
	char filename[] = "XMG00.jpg";
	if (!_Arduino_num)
		filename[0] = 1;
	else
		filename[0] = 2;

	for (uint8_t i = 0; i < 100; i++) {
		filename[3] = i/10 + '0';
		filename[4] = i%10 + '0';
		if (!SD.exists(filename)) {
			Cam_File = SD.open(filename, FILE_WRITE);   // only open a new file if it doesn't exist
			break;
		}
	}
}

void OpenFiles::OpenFile_PTH() {
	char filename[] = "PTH00.txt";
	for (uint8_t i = 0; i < 100; i++) {
		filename[3] = i/10 + '0';
		filename[4] = i%10 + '0';
		if (!SD.exists(filename)) {
			PTH_File = SD.open(filename, FILE_WRITE);   // only open a new file if it doesn't exist
			break;
		}
	}
}



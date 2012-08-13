#ifndef tetrapharmakos_h
#define tetrapharmakos_h

#define XBEE_RX 8
#define XBEE_TX 9

#include <Arduino.h>
#include <SD.h>

#define CHIP_SELECT 10

/* Ouverture des fichiers qui vont contenir les données aquises */
class OpenFiles {
	public:
		OpenFiles(bool Arduino_num);
		~OpenFiles();
		File Cam_File, PTH_File;

	private:
		bool _Arduino_num;
		void OpenFile_SD();
		void OpenFile_PTH();
};

#endif

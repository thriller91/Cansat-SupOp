#ifndef LinkSprite_h
#define LinkSprite_h


#include <SD.h>


/* Camera Jpeg LinkSprite */

class LinkSprite {
	public:
		LinkSprite();
		~LinkSprite();
		void Snap();
		void Save(File logfile);
	private:
		void SendResetCmd();
		void ReadImageSizeCmd();
		void SetImageSizeCmd();
		void SetBaudRateCmd();
		void SendTakePhotoCmd();
		void SendReadDataCmd();
		void StopTakePhotoCmd();
};

#endif

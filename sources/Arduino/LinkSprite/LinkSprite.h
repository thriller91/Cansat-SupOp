#ifndef LinkSprite_h
#define LinkSprite_h


#include <SD.h>


/* Camera Jpeg LinkSprite */

void CamStart();
void SaveToFile(File f);

void SendResetCmd();
void ReadImageSizeCmd();
void SetImageSizeCmd();
void SetBaudRateCmd();
void SendReadDataCmd();
void SendTakePhotoCmd();
void StopTakePhotoCmd();


#endif

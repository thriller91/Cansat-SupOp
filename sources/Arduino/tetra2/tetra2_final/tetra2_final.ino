#include <SD.h>
#include <Wire.h>
#include <SoftwareSerial.h>

#include "tetrapharmakos.h"

#include "LinkSprite.h"
//#include "BMP085.h" ////////// À décommenter pour activer le BMP

#define INTERRUPT_PIN 1
#define PTH_FILENAME "PTH.TXT"

#define IMG_FILENAME "IMG_2.JPG"
#define READ_FILENAME "IMG36.JPG"

//#define LG_DATA 6

#define INTERRUPT_PIN 1

SoftwareSerial InterArduino(6,7);
//uint8_t data[LG_DATA]={0,0,0,0,0,0};

File PTH_File, Cam_File, dataFile;


// interrupt
volatile unsigned long start_missions = 0;
volatile boolean start_flag = false;
int count_snap=7;
int count=0;

void setup(){
	//int i;
	// Communication
	Serial.begin(38400);
	CamStart();


	InterArduino.begin(300);
	pinMode(4, OUTPUT);
	pinMode(5, INPUT);
	digitalWrite(4, LOW);


	// SD Initialisation
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT)) {
		while(true); // ça sert à rien de continuer
	}


	SendTakePhotoCmd();

	if(SD.exists(IMG_FILENAME))
		SD.remove(IMG_FILENAME);
	Cam_File = SD.open(IMG_FILENAME, FILE_WRITE);

	//SaveToFile(Cam_File);

	Cam_File.close();
	ShutDownCmd();

	File dataFile = SD.open(READ_FILENAME);

	//delay(5000);
	digitalWrite(4, HIGH);
	while(digitalRead(5) == LOW) {
	}
	delay(100);
	if (dataFile) {
		while (dataFile.available()) {
			/*for(i=0;i<LG_DATA;i++){
				if (dataFile.available())
					data[i]=dataFile.read();
				else
					break;
			}
			InterArduino.write(data,LG_DATA);
			*/
			InterArduino.write(dataFile.read());
			//Serial.write(dataFile.read());
		}
	}

	dataFile.close();
	digitalWrite(4, LOW);

}

void loop(){
}


void start_counter() {
  start_missions = micros();
  start_flag = true;
  detachInterrupt(INTERRUPT_PIN);
}

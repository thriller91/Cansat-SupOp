/*
Programme à embarquer sur l'Arduino Mini n°2 du τετραφάρμακος.
TODO
-gérer la communication avec l'Arduino Mini n°1.
*/

#include <Wire.h>
#include <SD.h>
#include <SoftwareSerial.h>

#include "tetrapharmakos.h"

#include "LinkSprite.h"
#include "ADXL345.h"
#include "TinyGPS.h"

#define POS_FILENAME "POS.TXT"

//SoftwareSerial XBee(XBEE_RX,XBEE_TX);
SoftwareSerial InterArduino(7, 6); // RX, TX

#define IMG_FILENAME "IMG02.JPG"
#define POS_FILENAME "POS.TXT"

File POS_File, Cam_File, dataFile;

// GPS
TinyGPS gps;
SoftwareSerial ss(8, 9);

// ADXL345
ADXL345 accel;

// interrupt
volatile unsigned long start_missions = 0;
volatile boolean start_flag = false;



void setup(){
	// Communication
	Serial.begin(9600);
	Serial.print("Debut");
//////CamStart();
	attachInterrupt(INTERRUPT_PIN, start_counter, RISING);

	pinMode(5, INPUT);
	pinMode(4, OUTPUT);
	digitalWrite(4, LOW);

	// SD Initialisation
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT)) {
		Serial.println("SD initialization failed!");
		while(true); // ça sert à rien de continuer
	}

	Serial.println("SD OK");

	// GPS
	bool newData = false;
	unsigned long chars;
	unsigned short sentences, failed;
	ss.begin(4800);

	Serial.print("GPS OK");

	// ADXL345
	Wire.begin();
	accel = ADXL345();
	// Check that the accelerometer is infact connected.
	if(accel.EnsureConnected())
		Serial.println("Connected to ADXL345.");
	else
		Serial.println("Could not connect to ADXL345.");

	// Set the range of the accelerometer to a maximum of 2G.
	accel.SetRange(2, true);
	// Tell the accelerometer to start taking measurements.
	accel.EnableMeasurements();

	// Attente de largage ...
	Serial.print("Attente...");
	//

	while(!start_flag);

	Serial.println("Go!");

	if(SD.exists(POS_FILENAME))
		SD.remove(POS_FILENAME);
	POS_File = SD.open(POS_FILENAME, FILE_WRITE);

	while ((micros()-start_missions) < ((unsigned long) 10000000)) {//stop_missions_time) {
		/*
		Sortie série de la position GPS
		*/
		newData = false;

		// For one second we parse GPS data and report some key values
		for (unsigned long start = millis(); millis() - start < 1000;) {
			while (ss.available()) {
				char c = ss.read();
				// Serial.write(c); // uncomment this line if you want to see the GPS data flowing
				if (gps.encode(c)) // Did a new valid sentence come in?
					newData = true;
			}
		}

		if (newData) {
			float flat, flon;
			unsigned long age;
			gps.f_get_position(&flat, &flon, &age);
			POS_File.print("LAT=");
			POS_File.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
			POS_File.print(" LON=");
			POS_File.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
			POS_File.print(" SAT=");
			POS_File.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
			POS_File.print(" PREC=");
			POS_File.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
		}

		/*
		Sortie série de l'accélération du capteur ADXL345
		*/

		if(accel.IsConnected) { // If we are connected to the accelerometer.
			// Read the raw data from the accelerometer.
			AccelerometerRaw raw = accel.ReadRawAxis();
			//This data can be accessed like so:
			int xAxisRawData = raw.XAxis;
			// Read the *scaled* data from the accelerometer (this does it's own read from the accelerometer
			// so you don't have to ReadRawAxis before you use this method).
			// This useful method gives you the value in G thanks to the Love Electronics library.
			AccelerometerScaled scaled = accel.ReadScaledAxis();
			// This data can be accessed like so:
			float xAxisGs = scaled.XAxis;
			// We output our received data.
			Output(raw, scaled);
		}
	}
	POS_File.close();

	InterArduino.begin(4800);
	dataFile = SD.open(POS_FILENAME);

	while(digitalRead(5) == LOW);


	digitalWrite(4, HIGH);
	delay(50);
	if (dataFile) {
		while (dataFile.available())
			InterArduino.write(dataFile.read());
	}

	else
		InterArduino.println("Erreur d'ouverture de POS.TXT en lecture sur Arduino2");

	digitalWrite(4, LOW);
	dataFile.close();

}

void loop(){
	Serial.println("Fin");
	delay(1000);
}


void start_counter() {
  start_missions = micros();
  start_flag = true;
  detachInterrupt(INTERRUPT_PIN);
}

// Output the data down the serial port.
void Output(AccelerometerRaw raw, AccelerometerScaled scaled) {
	POS_File.print(scaled.XAxis);
	POS_File.print("\t");
	POS_File.print(scaled.YAxis);
	POS_File.print("\t");
	POS_File.println(scaled.ZAxis);
}

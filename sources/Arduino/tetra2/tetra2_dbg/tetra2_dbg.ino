/*
Programme de test des composants à embarquer sur l'Arduino Mini n°2 du τετραφάρμακος.
*/

#include <Wire.h>
#include <SoftwareSerial.h>

#include "ADXL345.h"
#include "TinyGPS.h"

TinyGPS gps;
SoftwareSerial ss(8, 9);

ADXL345 accel;

void setup(){
	bool newData = false;
	unsigned long chars;
	unsigned short sentences, failed;

	// Communication
	Wire.begin();
	Serial.begin(9600);
	ss.begin(4800);
	Serial.println("Debut");
	delay(1000);


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

	while (true){
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
			Serial.print("LAT=");
			Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
			Serial.print(" LON=");
			Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
			Serial.print(" SAT=");
			Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
			Serial.print(" PREC=");
			Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
		}

		gps.stats(&chars, &sentences, &failed);
		Serial.print(" CHARS=");
		Serial.print(chars);
		Serial.print(" SENTENCES=");
		Serial.print(sentences);
		Serial.print(" CSUM ERR=");
		Serial.println(failed);

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
}

void loop(){
}

// Output the data down the serial port.
void Output(AccelerometerRaw raw, AccelerometerScaled scaled) {
	// Tell us about the raw values coming from the accelerometer.
	Serial.print("Raw:\t");
	Serial.print(raw.XAxis);
	Serial.print("   ");
	Serial.print(raw.YAxis);
	Serial.print("   ");
	Serial.print(raw.ZAxis);

	// Tell us about the this data, but scale it into useful units (G).
	Serial.print("   \tScaled:\t");
	Serial.print(scaled.XAxis);
	Serial.print("G   ");
	Serial.print(scaled.YAxis);
	Serial.print("G   ");
	Serial.print(scaled.ZAxis);
	Serial.println("G");
}

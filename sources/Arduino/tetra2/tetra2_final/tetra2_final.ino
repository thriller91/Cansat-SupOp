/*
Programme à embarquer sur l'Arduino Mini n°2 du τετραφάρμακος.
TODO
-gérer la communication avec l'Arduino Mini n°1.
*/

#include <Wire.h>
#include <SD.h>

#include "tetrapharmakos.h"

#include "ADXL345.h"

#define POS_FILENAME "POS.TXT"

File POS_File;

// ADXL345
ADXL345 accel;

// interrupt
volatile unsigned long start_mission = 0;
volatile boolean start_flag = false;

int count = 0;



void setup(){
        Serial.begin(9600);
        Serial.println("Debut");
        
        pinMode(INTERRUPT_PIN,INPUT);
	attachInterrupt(INTERRUPT_PIN, start_counter, RISING);

	// SD Initialisation
	pinMode(10, OUTPUT);

	if (!SD.begin(CHIP_SELECT)) {
		while(!SD.begin(CHIP_SELECT)) { // ça sert à rien de continuer
                          Serial.println("No SD");
                          delay(10000);
                }
                
	}

	Serial.println("SD OK");

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

}

void loop(){
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
    count++;
    if (count>50) {
       count = 0;
       POS_File.close();
       POS_File = SD.open(POS_FILENAME, FILE_WRITE);
       Serial.println("+50");
    }
    delay(10); 
}


void start_counter() {
  start_mission = millis();
  start_flag = true;
  detachInterrupt(INTERRUPT_PIN);
}

// Output the data down the serial port.
void Output(AccelerometerRaw raw, AccelerometerScaled scaled) {
	POS_File.print(scaled.XAxis);
	POS_File.print("\t");
	POS_File.print(scaled.YAxis);
	POS_File.print("\t");
	POS_File.print(scaled.ZAxis);
	POS_File.print("\t");
        POS_File.println(millis()-start_mission);
}

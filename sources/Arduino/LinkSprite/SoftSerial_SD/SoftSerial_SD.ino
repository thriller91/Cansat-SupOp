/*
  SD card read/write
 
 This example shows how to read and write data to and from an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */
 
#include <SD.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3);
File myFile;

void setup()
{
 // Open serial communications and wait for port to open:
  mySerial.begin(9600);

  //delay(3000);
  //Serial.print("Initializing SD card...");
  // On the Ethernet Shield, CS is pin 4. It's set as an output by default.
  // Note that even if it's not used as the CS pin, the hardware SS pin 
  // (10 on most Arduino boards, 53 on the Mega) must be left as an output 
  // or the SD library functions will not work. 
   pinMode(10, OUTPUT);
   
  if (!SD.begin(10)) {
    mySerial.println("initialization failed!");
    return;
  }
  //Serial.println("initialization done.");
  
  // re-open the file for reading:
  myFile = SD.open("IMG00.JPG");
  if (myFile) {
    //Serial.println("test.txt:");
    
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
    	mySerial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
  	// if the file didn't open, print an error:
    mySerial.println("error opening test.txt");
  }
}

void loop()
{
	// nothing happens after setup
}



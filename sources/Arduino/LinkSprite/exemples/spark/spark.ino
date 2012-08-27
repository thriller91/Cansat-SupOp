#include <SD.h>
#include <SoftwareSerial.h>

byte incomingbyte;                    
byte a[32];                            //Array to store image chuncks to be read off the camera
int x=0x0000,j=0,k=0;                  //Read Starting address   
uint8_t MH=0x00,ML=0x00;
boolean EndFlag=0;
int chipSelect = 10;                    //Required when using Sparkfun SD shield
File logfile;

SoftwareSerial XBee(8,9);

void SendResetCmd();
void ReadImageSizeCmd();
void SetBaudRateCmd();
void SetImageSizeCmd();
void SendTakePhotoCmd();
void SendReadDataCmd();
void StopTakePhotoCmd();

void OpenFile();



void setup()
{ 
	XBee.begin(9600);
	XBee.println("Debut");
	Serial.begin(38400);    
	
	pinMode(10, OUTPUT);        // make sure that the default chip select pin is set to output, even if you don't use it
  
	//pinMode(13, OUTPUT);        // LED pin to show that photo has been taken.
	//digitalWrite(13, LOW);
  
	SD.begin(chipSelect);
}
  
void loop() 
{
	OpenFile();              // Function to create a new file (IMGXX.jpeg)
	x=0x0000;
	x-=0x20;
	EndFlag=0;
     
	XBee.println("Reset Cam");
	SendResetCmd();                  // Reset the Camera
	delay(3000);                     //After reset, wait 2-3 second to send take picture command 

	XBee.println("Snap!");
	SendTakePhotoCmd(); 
	while(Serial.available()>0)
	{
		incomingbyte=Serial.read();
	}
      
	while(!EndFlag)
	{  
		j=0;
		k=0;

		SendReadDataCmd();
		delay(15);
		    
		while(Serial.available()>0)
		{
			incomingbyte=Serial.read();
			k++;               
			if((k>5)&&(j<32)&&(!EndFlag))
			{
				a[j]=incomingbyte;
				if((a[j-1]==0xFF)&&(a[j]==0xD9))      //Check if the picture is over
					EndFlag=1; 
				logfile.print((char)incomingbyte);                  
				j++;
			}
		}      
	}
	logfile.close();  
	//digitalWrite(13, HIGH);                        // Turn LED on to indicate the end of image transfer
	while(1) {
		XBee.println("Fin");
		delay(1000);
	}
}

// Function to create a new image name every time
void OpenFile()
{
 char filename[] = "IMG00.jpg";
 for (uint8_t i = 0; i < 100; i++) {
      filename[3] = i/10 + '0';
      filename[4] = i%10 + '0';
      if (!SD.exists(filename)) {
        logfile = SD.open(filename, FILE_WRITE);   // only open a new file if it doesn't exist
        break;                                     // leave the loop!
      }
  }  
}  


//Camera functions

//Send Reset command
void SendResetCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x26);
      Serial.write(byte(0x00));
}

// Send image size command, image size is returned
void ReadImageSizeCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x34);
      Serial.write(0x01);
      Serial.write(byte(0x00));
}

// Set image size
void SetImageSizeCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x31);
      Serial.write(0x05);
      Serial.write(0x04);
      Serial.write(0x01);
      Serial.write(byte(0x00));
      Serial.write(0x19);
      Serial.write(0x22);
}

//Set up the Baud Rate of the camera
void SetBaudRateCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x24);
      Serial.write(0x03);
      Serial.write(0x01);
      Serial.write(0xAE);
      Serial.write(0xC8);

}

//Send take picture command
void SendTakePhotoCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x36);
      Serial.write(0x01);
      Serial.write(byte(0x00));  
}

//Read data
void SendReadDataCmd()
{

      MH=x/0x100;
      ML=x%0x100; 
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x32);
      Serial.write(0x0c);
      Serial.write(byte(0x00)); 
      Serial.write(0x0a);
      Serial.write(byte(0x00));
      Serial.write(byte(0x00));
      Serial.write(MH);
      Serial.write(ML);  
      Serial.write(byte(0x00));
      Serial.write(byte(0x00));
      Serial.write(byte(0x00));
      Serial.write(0x20);
      Serial.write(byte(0x00));  
      Serial.write(0x0a);
      x+=0x20;                            //address increases 32£¬set according to buffer size
}

//Stop taking a picture
void StopTakePhotoCmd()
{
      Serial.write(0x56);
      Serial.write(byte(0x00));
      Serial.write(0x36);
      Serial.write(0x01);
      Serial.write(0x03);        
}

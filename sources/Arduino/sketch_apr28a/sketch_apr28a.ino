#include <cansatComm.h>

#include <SoftwareSerial.h>

SoftwareSerial cam(5,4);
cansatComm comm(0x04,0x10);

uint8_t commutCam=9;

char ID0=0xAA;
char ID_INITIAL=0x01;
char ID_GET_PICTURE=0x04;
char ID_SNAPSHOT=0x05;
char ID_SET_PACKAGE_SIZE=0x06;
char ID_SetBaudRate=0x07;
char ID_RESET=0x08;
char ID_DATA=0x0A;
char ID_SYNC=0x0D;
char ID_ACK=0x0E;
char ID_NAK=0x0F;
char ID_LIGHT=0x13;

uint16_t pkgsize=64;

void setup()
{
  Serial.begin(115200);
  comm.begin(111111,5);
  int k=0;
  pinMode(commutCam,OUTPUT);
  digitalWrite(commutCam,LOW);
  char PROGMEM sync[6]={ID0,ID_SYNC,0x00,0x00,0x00,0x00};
  char PROGMEM init[6]={ID0, ID_INITIAL,0x00, 0x07, 0x09, 0x05};
  char ACK[6]={ID0,ID_ACK,0x00,0x00,0x00,0x00};
  char PROGMEM snapshot[6]={ID0,ID_SNAPSHOT,0x00,0x00,0x00,0x00};
  char setpkgsize[6]={ID0,ID_SET_PACKAGE_SIZE,0x08,0x40,0x00,0x00};
  char PROGMEM getpicture[6]={ID0,ID_GET_PICTURE,0x01,0x00,0x00,0x00};
  char setbaudrate[6]={ID0,ID_SetBaudRate,0x0A,0x02,0x00,0x00};
  delay(1000);
  
  char imgPkg[pkgsize];
  
  //synchronisation
  while (k<40)
  {
    comm.sendText("sync ");
    comm.sendText(k);
    for (int i=0;i<6;i++)
    {
      Serial.print(sync[i]);
    }
    delay(1000);
    if (Serial.available()) break;
    k++;
  }
  //recup ACK
  while (Serial.available())
  {
    comm.sendText(Serial.read(),HEX);
    //comm.sendText(" ");
  }  
  //comm.sendText("");
  
  delay(400);
  
  //envoi ACK
  for (int i=0;i<6;i++)
  {
    Serial.print(ACK[i]);
  }  
  
  delay(400);
 
  //set baud rate 
  comm.sendText("set baud rate");
  for (int i=0;i<6;i++)
  {
    Serial.print(setbaudrate[i]);
  }
    //recup ACK
  while (Serial.available())
  {
    comm.sendText(Serial.read(),HEX);
    //comm.sendText(" ");
  }  
  Serial.end();
  Serial.begin(111111);
  
  delay(400);
  
  //initialisation
  ("init");
  for (int i=0;i<6;i++)
  {
    Serial.print(init[i]);
  }
  //recup ACK
  while (!Serial.available()){};
  while (Serial.available())
  {
    comm.sendText(Serial.read(),HEX);
    //comm.sendText(" ");
  }
  //comm.sendText("");
  
  delay(2000);
  
  //snapshot
  comm.sendText("snapshot");
  for (int i=0;i<6;i++)
  {
    Serial.print(snapshot[i]);
  }
  comm.setImgDate();
  //recup ACK
  while (!Serial.available()){};
  while (Serial.available())
  {
    comm.sendText(Serial.read(),HEX);
    //comm.sendText(" ");
  }
  //comm.sendText("");
  
  delay(400);
  
  //set package size
  comm.sendText("Set pkg size");
  setpkgsize[3]=pkgsize & 0xFF;
 setpkgsize[4]=pkgsize>>8;
  for (int i=0;i<6;i++)
  {
    Serial.print(setpkgsize[i]);
  }  
  //recup ACK
  while (!Serial.available()){};
  while (Serial.available())
  {
    comm.sendText(Serial.read(),HEX);
   // comm.sendText(" ");
  }
  //comm.sendText("");
  
  delay(400);
  
  //get picture
  comm.sendText("Get picture");
  for (int i=0;i<6;i++)
  {
    Serial.print(getpicture[i]);
  }  
  //recup ACK
  while (!Serial.available()){};
  for (int i=0;i<9;i++)
  {
    comm.sendText(Serial.read(),HEX);
    //comm.sendText(" ");
  }
  uint8_t datalength[3];
  for (int i=0;i<3;i++)
  {
    datalength[i]=Serial.read();
    comm.sendText(datalength[i],HEX);
    //comm.sendText(" ");
  }
  //comm.sendText("");
  
  unsigned long imagesize=0;
  for (int i=0;i<3;i++)
  {
    imagesize += datalength[i]<<8*i;
  }
  //comm.sendText("image size = ");
  //comm.sendText(imagesize);
  unsigned int nbpkg=imagesize/(pkgsize-6)+1;
  //comm.sendText("nb pkg = ");
  //comm.sendText(nbpkg);
  
  comm.setimgid(0x01);
  comm.setnbPkg(nbpkg);
  comm.setPkgSize(pkgsize);
  
  comm.headerImgDate();
  
  for (int i=0;i<nbpkg;i++)
  {
    Serial.flush();
    delay(50);
    ACK[4]=i;
    
    for (int i=0;i<6;i++)
    {
      Serial.print(ACK[i]);
    }
    
    while (!Serial.available()){};
    
    comm.headerImgPkg();
    comm.SendImgPkg();
    
  }

      
   
}

void loop()
{
  
}

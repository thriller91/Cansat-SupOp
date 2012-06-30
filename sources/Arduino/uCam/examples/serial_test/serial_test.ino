#define UCAM_ID0 0xAA
#define UCAM_ID_SYNC 0x0D

uint8_t sync[6]={UCAM_ID0,UCAM_ID_SYNC,0x00,0x00,0x00,0x00};



void setup(){
	Serial.begin(115200);
}

void loop(){
	Serial.write(sync, 6);
	delay(1000);
}

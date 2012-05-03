#include <boost/asio.hpp> // include boost
using namespace::boost::asio;  // save tons of typing
#include <iostream>
using namespace std;
#include <ctime>

// These are the values our port needs to connect

	const char *PORT(0);
	
// Note: all the following except BAUD are the exact same as the default values

// what baud rate do we communicate at
serial_port_base::baud_rate BAUD(115200);
// how big is each "packet" of data (default is 8 bits)
serial_port_base::character_size CSIZE2( 8 );
// what flow control is used (default is none)
serial_port_base::flow_control FLOW( serial_port_base::flow_control::none );
// what parity is used (default is none)
serial_port_base::parity PARITY( serial_port_base::parity::none );
// how many stop bits are used (default is one)
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one );

int main(int argc, char * argv[])
{

	unsigned char response[6];
	unsigned char c;
	unsigned short r;
	unsigned char headerimgpkg[4];
	unsigned int pkgSizeIm;
	unsigned int pkgId;
	
	int k;

	uint8_t ims;

	char * pEnd;
	if (argc == 2)
	{
		PORT = argv[1];
	}
	else
	{
		return 1;
	}
	
	
	// create the I/O service that talks to the serial device
	io_service io;
	// create the serial device, note it takes the io service and the port name
	serial_port port( io, PORT );

	// go through and set all the options as we need them
	// all of them are listed, but the default values work for most cases
	port.set_option( BAUD );
	port.set_option( CSIZE2 );
	port.set_option( FLOW );
	port.set_option( PARITY );
	port.set_option( STOP );

	unsigned char ID0=0xAA;
	unsigned char ID_INITIAL=0x01;
	unsigned char ID_GET_PICTURE=0x04;
	unsigned char ID_SNAPSHOT=0x05;
	unsigned char ID_SET_PACKAGE_SIZE=0x06;
	unsigned char ID_SetBaudRate=0x07;
	unsigned char ID_RESET=0x08;
	unsigned char ID_DATA=0x0A;
	unsigned char ID_SYNC=0x0D;
	unsigned char ID_ACK=0x0E;
	unsigned char ID_NAK=0x0F;
	unsigned char ID_LIGHT=0x13;
	
	unsigned char sync[6]={ID0,ID_SYNC,0x00,0x00,0x00,0x00};
	unsigned char init[6]={ID0, ID_INITIAL,0x00, 0x07, 0x09, 0x05};
	unsigned char ACK[6]={ID0,ID_ACK,0x00,0x00,0x00,0x00};
	unsigned char snapshot[6]={ID0,ID_SNAPSHOT,0x00,0x00,0x00,0x00};
	unsigned char setpkgsize[6]={ID0,ID_SET_PACKAGE_SIZE,0x08,0xC8,0x00,0x00};
	unsigned char getpicture[6]={ID0,ID_GET_PICTURE,0x01,0x00,0x00,0x00};
	
	time_t date;
	
	write( port, buffer( sync, 6 ) );
	
	for (k=0;k<12;k++)
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;
	
	
	write( port, buffer( ACK, 6 ) );
	
	date = time(NULL);
	while (time(NULL)-date < 1){}
	
	write( port, buffer( init, 6) );
	
	for (k=0;k<6;k++)
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;
	
	date = time(NULL);
	while (time(NULL)-date < 2){}
	
	write( port, buffer( snapshot,6 ) );
	for (k=0;k<6;k++)
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;
	
	write( port, buffer( setpkgsize,6 ) );
	for (k=0;k<6;k++)
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;
	
	write( port, buffer( getpicture,6 ) );
	for (k=0;k<6;k++)
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	for (k=0;k<6;k++)
	{
		read(port, buffer( &c,1 ) );
		response[k] = c;
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;
	
	//unsigned int imgSize = (response[3]&0xFF) + (response[4]&0xFF)<<8 + (response[5]&0xFF)<<16;
	
	unsigned int imgSize=0;
	for (k=0;k<3;k++)
	{
		ims=response[k+3];
		imgSize+=ims<<(8*k);
	}
	
	//cout<<"Image size : "<<dec<<imgSize;
	
	unsigned int nbpkg = ceil(static_cast<float>(imgSize)/194);
	
	//cout<<" , number of package : "<<nbpkg<<endl;
	
	date = time(NULL);
	while (time(NULL)-date < 1){}
	
	ACK[2] = ID_DATA;
	
	
	for (k=0;k<nbpkg;k++)
	{
		ACK[4]=k;//(k&0xFF);
		r=ACK[4];
		//cout<<r<<endl;
		//ACK[5]=(k>>8)&0xFF;
		write( port, buffer( ACK,6 ) );
		for (int i=0;i<4;i++)
		{
			read(port, buffer( &c,1 ) );
			r=c;
			headerimgpkg[i]=c;
			//cout<<hex<<r<<" ";
		}
		
		ims = headerimgpkg[0];
		pkgId = ims;
		ims = headerimgpkg[1];
		pkgId += ims<<8;
		ims = headerimgpkg[2];
		pkgSizeIm = ims;
		ims = headerimgpkg[3];
		pkgSizeIm += ims<<8;
		
		//cout<<dec<<"	"<<pkgId<<"	"<<pkgSizeIm<<endl;
		for (int i=0;i<pkgSizeIm;i++)
		{
			read(port, buffer( &c,1 ) );
			r=c;
			//cout<<hex<<r<<" ";
			cout<<c;
		}
		read(port, buffer( &c,1 ) );
		read(port, buffer( &c,1 ) );
		
	}
	
	
	return 0;
}

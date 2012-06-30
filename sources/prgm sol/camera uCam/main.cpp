#include <boost/asio.hpp>
using namespace::boost::asio;
#include <iostream>
#include <fstream>
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
	unsigned char headerimgpkg[4];
	//unsigned short r;
	unsigned char r;
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

	//Command identifiers bytes
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

	//commands
	unsigned char sync[6]={ID0,ID_SYNC,0x00,0x00,0x00,0x00};
	unsigned char init[6]={ID0, ID_INITIAL,0x00, 0x07, 0x09, 0x05};
	unsigned char ACK[6]={ID0,ID_ACK,0x00,0x00,0x00,0x00};
	unsigned char snapshot[6]={ID0,ID_SNAPSHOT,0x00,0x00,0x00,0x00};
	unsigned char setpkgsize[6]={ID0,ID_SET_PACKAGE_SIZE,0x08,0xC8,0x00,0x00};
	unsigned char getpicture[6]={ID0,ID_GET_PICTURE,0x01,0x00,0x00,0x00};
	unsigned char reset[6]={ID0,ID_RESET,0x00,0x00,0x00,0xFF};

	time_t date;

	//cout<<"Sync ...";
	write( port, buffer( sync, 6 ) );	//sends sync packet to the camera
	//printf("%d",sync);
	//cout<<" OK"<<endl;


	for (k=0;k<12;k++)	//recovers serial bytes from the camera. Does not check it, simply empties buffer and waits for all the bytes to be received.
	{
		//cout<<"Read ... "<<endl;
		read(port, buffer( &c,1 ) );
		//cout<<"OK"<<endl;
		r=c;
		//cout<<r<<" ";
	}
	//cout<<endl;

	//cout<<endl;
	write( port, buffer( ACK, 6 ) );	//sends aknowledgement command once response has been received

	date = time(NULL);
	while (time(NULL)-date < 1){}	//waits 1 second before continuing

	write( port, buffer( init, 6) );	//sends initialization command.

	for (k=0;k<6;k++)	//recovers serial bytes from the camera. Does not check it, simply empties buffer and waits for all the bytes to be received.
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;

	date = time(NULL);
	while (time(NULL)-date < 2){}	//waits 2 seconds

	write( port, buffer( snapshot,6 ) );	//sends snapshot command
	for (k=0;k<6;k++)
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;

	write( port, buffer( setpkgsize,6 ) );	//sends setpkgsize command
	for (k=0;k<6;k++)
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;

	write( port, buffer( getpicture,6 ) );	//set get picture command
	for (k=0;k<6;k++)	//recovers serial ACK from the camera. Does not check it, simply empties buffer and waits for all the bytes to be received.
	{
		read(port, buffer( &c,1 ) );
		r=c;
		//cout<<hex<<r<<" ";
	}
	for (k=0;k<6;k++)	//recovers data package form the camera and saves it. Dos not check if it is a valid package though.
	{
		read(port, buffer( &c,1 ) );
		response[k] = c;
		r=c;
		//cout<<hex<<r<<" ";
	}
	//cout<<endl;


	unsigned int imgSize=0;
	for (k=0;k<3;k++)	//reads package size thanks to the data package
	{
		ims=response[k+3];
		imgSize+=ims<<(8*k);
	}

	//cout<<"Image size : "<<dec<<imgSize;

	unsigned int nbpkg = ceil(static_cast<float>(imgSize)/194);	//computes number of package to receive.

	//cout<<" , number of package : "<<nbpkg<<endl;

	date = time(NULL);
	while (time(NULL)-date < 1){}	//waits 1 second

	ACK[2] = ID_DATA;	//sets ACK package to ask image package to the camera.


	for (k=0;k<nbpkg;k++)
	{
		ACK[4]=k;//(k&0xFF);	//id of the package to recover. Here we assume there i sless than 255 packages.
		r=ACK[4];
		//cout<<r<<endl;
		//ACK[5]=(k>>8)&0xFF;
		write( port, buffer( ACK,6 ) );
		for (int i=0;i<4;i++)	//saves the 4 first bytes of the package which contain the id and the image data size
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
		for (int i=0;i<pkgSizeIm;i++)	//reads the image data bytes and sends it to the standard output.
		{
			read(port, buffer( &c,1 ) );
			r=c;
			//cout<<hex<<r<<" ";
			//cout<<c;
		}
		read(port, buffer( &c,1 ) );
		read(port, buffer( &c,1 ) );

	}

	write( port, buffer( reset,6 ) );
	//cout<<endl;

	return 0;
}

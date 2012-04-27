#include <boost/asio.hpp> 
using namespace::boost::asio;
#include <iostream>
using std::cin;
using std::cout;
#include <string>
using std::string;
#include <fstream>
//#include "receiveFile.h"

using namespace std;
using namespace::boost::asio;

///////////////////
void receiveFile(const char * filename, long length, serial_port port)
{
	char * buf;
	ofstream myFile(filename, ios::out | ios::binary);
	char c;
	buf=new char[length];
	if (myFile)
	{
		for (long k;k<length;k++)
		{
			read(port,boost::asio::buffer(&c,1));
			buf+=c;
		}
		myFile.write(buf,length);
	}
	else
	{
		cout<<"Error : Unable to open "<<filename<<endl;
	}
}
/////////////////:

const char *PORT = "/dev/ttyACM0";

// what baud rate do we communicate at
serial_port_base::baud_rate BAUD(460800);
// how big is each "packet" of data (default is 8 bits)
serial_port_base::character_size CSIZE2( 8 );
// what flow control is used (default is none)
serial_port_base::flow_control FLOW( serial_port_base::flow_control::none );
// what parity is used (default is none)
serial_port_base::parity PARITY( serial_port_base::parity::none );
// how many stop bits are used (default is one)
serial_port_base::stop_bits STOP( serial_port_base::stop_bits::one );

int main(void)
{
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
	
	string filename;
	filename="img_1.jpg";

	receiveFile(filename.c_str(),255701,port);
	
	return 0;
}

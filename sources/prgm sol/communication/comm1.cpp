#include <boost/asio.hpp> 
using namespace::boost::asio;
#include <iostream>
using std::cin;
using std::cout;
#include <string>
using std::string;

const char *PORT = "/dev/ttyUSB1";

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
	
	char c;
	string s;

	while (1)
	{
		for (;;)
		{
			read(port,buffer(&c,1));//s=c;
//			    switch(c)
//			    {
//				case '\r':
//				    break;
//				case '\n':
				    cout<<c;//<<std::endl;
//				default:
//				    s+=c;
//			    }
		}
	}
	
	return 0;
}

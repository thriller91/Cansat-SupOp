#include <boost/asio.hpp> 
#include <boost/lexical_cast.hpp>
using namespace::boost::asio;
using boost::lexical_cast;
#include <iostream>
#include <fstream>
using namespace std;
#include <string>
#include <sstream>

const char *PORT(0);

// what baud rate do we communicate at
serial_port_base::baud_rate BAUD(9600);


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

	ofstream TimeAtmAccData;
	ofstream GPS;
	ofstream img;
	ofstream Misc;

	char c;
	char whichdata;
	string s="";
	int nbr_line;
	int k=0;

	string filename;
	stringstream filename0;



	while (1){
		//read(port,buffer(&c,1));
		GPS.open("GPS.txt",ios::app);
		//cout<<"Nouveau paquet sans signature"<<endl;
		//read(port,buffer(&c,1));
        //c'est ici des données GPS, pour tester
		do
		{
			read(port,buffer(&c,1));
			s+=c;
			k++;
		}while (k<16);
		//cout<<s;
		GPS<<s;
		s="";
		GPS.close();
	}

	return 0;
}




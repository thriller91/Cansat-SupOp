#include <boost/asio.hpp> 
using namespace::boost::asio;
#include <iostream>
#include <fstream>
using namespace std;
#include <string>
using std::string;
#include "comm4.h"

const char *PORT(0);

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
	
	ofstream TimeAtmAccData("TimeAtmAccData.txt");
	ofstream GPS("GPS.txt");
	
	//if (!TimeAtmAccData) cout<<"Erreur à l'ouverture de TimeAtmAccData.txt"<<endl;
	//if (!GPS) cout<<"Erreur à l'ouverture de GPS.txt"<<endl;
	
	//TimeAtmAccData<<"aaa"<<endl;
	//GPS<<"aaa"<<endl;
	
	char c;
	char whichdata;
	string s="";
	int nbr_line;
	
	uint8_t cam,imgId;
	uint32_t imgDate;
	uint8_t temp;
	uint16_t nbPkg;
	
	uint16_t pkgId;
	uint16_t pkgSize;

	char ** imgPkg(0);

	while (1)
	{
		read(port,buffer(&c,1));
		if(!isHeader(c))
		{
		
		}
		else
		{
			cout<<"Nouveau paquet"<<endl;
			read(port,buffer(&c,1));
			switch (c)
			{
				case 0x06:
					ofstream TimeAtmAccData("TimeAtmAccData.txt",ios::app);
					ofstream GPS("GPS.txt",ios::app);
				
					read(port,buffer(&whichdata,1));
					nbr_line = Rnbr(whichdata);
				
					if (nbr_line > 0)
					{
						cout<<"Données numériques"<<endl;
						do
						{
						    read(port,buffer(&c,1));
						    s+=c;					    
						}while (c != '\n');
						cout<<s;
						TimeAtmAccData<<s.c_str();
						s="";
					}
					if (nbr_line > 1)
					{
						cout<<"Données GPS"<<endl;
						do
						{
						    read(port,buffer(&c,1));
						    s+=c;					    
						}while (c != '\n');
						cout<<s;
						GPS<<s;
						s="";
					}
					TimeAtmAccData.close();
					GPS.close();
					
					imgPkg = new char*[nbPkg];
					
					
				case 10:
					read(port,buffer(&c,1));
					cam = 0x80 & c;
					imgId = 0x7F & c;
					cout<<"receiving image "<<imgId<<" from camera"<<cam<<endl;
					imgDate = 0;
					for (int i=0;i<4;i++)
					{
						read(port,buffer(&c,1));
						temp = c;
						imgDate += temp<<(8*i);
					}
					nbPkg=0;
					for (int i=0;i<2;i++)
					{
						read(port,buffer(&c,1));
						temp = c;
						nbPkg += temp<<(8*i);
					}
					
					
					
					
					
				case 01:
					read(port,buffer(&c,1));
					cam = 0x80 & c;
					imgId = 0x7F & c;
					
					string filename = "img_" + cam + "_" + imgId;
					ofstream img(filename.c_str(),ios::app | ios::binary);
					
					pkgId = 0;
					for (int i=0;i<2;i++)
					{
						read(port,buffer(&c,1));
						temp = c;
						imgId += temp<<(8*i);
					}
					
					for (int i=0;i<2;i++)
					{
						read(port,buffer(&c,1));
						temp = c;
						pkgSize += temp<<(8*i);
					}
					
					imgPkg[pkgId] = new char[pkgSize];
					
					for (int l=0; l<pkgSize; l++)
					{
						read(port,buffer(&c,1));
						imgPkg[k][l] = c;
					}
					
			}
		}
	}
	
	return 0;
}




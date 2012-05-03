#include <boost/asio.hpp> 
#include <boost/lexical_cast.hpp>
using namespace::boost::asio;
using boost::lexical_cast;
#include <iostream>
#include <fstream>
using namespace std;
#include <string>
#include "comm4.h"
#include <sstream>
#include <magick/api.h>
#include <magick/ImageMagick.h>

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
	
	ofstream TimeAtmAccData;
	ofstream GPS;
	ofstream img;
	
	char c;
	char whichdata;
	string s="";
	int nbr_line;
	
	uint8_t cam,imgId;
	uint32_t imgDate;
	uint8_t temp;
	uint16_t nbPkg;
	
	uint16_t pkgId;
	uint8_t pkgIdBytes[2];
	uint16_t pkgSize;
	uint8_t pkgSizeBytes[2];
	
	uint16_t nbRow,nbCol;
	uint8_t nbbitperpix;
	uint32_t imgSize;
	
	string filename;
	stringstream filename0;
	
	Image *Img(0);
	ExceptionInfo *exception;
	ImageInfo *imageinfo(0);
	
	exception=AcquireExceptionInfo();
	imageinfo=CloneImageInfo(NULL);
	
	InitializeMagick((char *) NULL);

	char * imgPkg(0);


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
					TimeAtmAccData.open("TimeAtmAccData.txt",ios::app);
					GPS.open("GPS.txt",ios::app);
				
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
					
					break;
					
					
					
					
				case 0x10:
					read(port,buffer(&c,1));
					cam = (0x80 & c)>>7;
					imgId = 0x7F & c;
					cout<<"receiving image "<<static_cast<int>(imgId)<<" from camera"<<static_cast<int>(cam)<<endl;
					imgDate = 0;
					for (int i=0;i<4;i++)
					{
						read(port,buffer(&c,1));
						temp = c;
						imgDate += temp<<(8*i);
					}
					cout<<"taken at "<<static_cast<unsigned long>(imgDate)<<"ms"<<endl;
					nbPkg=0;
					for (int i=0;i<2;i++)
					{
						read(port,buffer(&c,1));
						temp = c;
						nbPkg += temp<<(8*i);
					}
					for (int i=0;i<2;i++)
					{
						read(port,buffer(&c,1));
						temp = c;
						pkgSize += temp<<(8*i);
					}
					cout<<static_cast<int>(nbPkg)<<"	"<<static_cast<int>(pkgSize)<<endl;
					imgPkg= new char[pkgSize];
					break;
					
					
					
					
				case 0x01:
					read(port,buffer(&c,1));
					cam = 0x80 & c>>7;
					imgId = 0x7F & c;
					
					cout<<static_cast<int>(cam)<<"	"<<static_cast<int>(imgId)<<endl;
					
					filename="img_" + lexical_cast<string>(static_cast<int>(cam)) + "_" + lexical_cast<string>(static_cast<int>(imgId)) + ".jpg";

					
					cout<<filename<<endl;
					
					img.open(filename.c_str(),ios::app | ios::binary);
					
					cout<<pkgId<<endl;
					pkgId=0;
					pkgSize=64;
					
					for (int l=0; l<pkgSize; l++)
					{
						read(port,buffer(&c,1));
						imgPkg[l] = c;
					}
					
					if (checkPkg(imgPkg,pkgSize))
					{
						cout<<"Package "<<pkgId<<" checked"<<endl;
					}
					else
					{
						cout<<"Bad checksum on pkg "<<pkgId<<endl;
					}
					
					img.write(imgPkg+4,pkgSize-6);
					img.close();
					
					if (pkgId == nbPkg-1)
					{
						delete imgPkg;
					}
					
					break;
					
					
					
				
				case 0x02:
					while (c != '\n')
					{
					    read(port,buffer(&c,1));
					    s+=c;					    
					}
					cout<<s;
					break;					
					
					
				
				case 0x0A:
					read(port,buffer(&c,1));
					cam = 0x80 & c>>7;
					imgId = 0x7F & c;

					filename = "img_" + lexical_cast<string>(static_cast<int>(cam)) + "_" + lexical_cast<string>(static_cast<int>(imgId)) + ".png";
					
					
					
					nbRow = 0;
					for (int k=0;k<2;k++)
					{
						read(port,buffer(&c,1));
						temp = c;
						nbRow += temp;
					}
					
					nbCol = 0;
					for (int k=0;k<2;k++)
					{
						read(port,buffer(&c,1));
						temp = c;
						nbCol += temp;
					}
					
					nbCol=320;
					nbRow=240;
					
					
					read(port,buffer(&c,1));
					
					nbbitperpix = c;
					
					imgSize = nbCol*nbRow;
					
					imgPkg = new char[imgSize];
					

					
					for (int k=0; k<imgSize; k++)
					{
						
						read(port,buffer(&c,1));
						imgPkg[k] = c;
						cout<<k<<endl;
					}
					
					Img = ConstituteImage(nbCol,nbRow,"I",CharPixel,imgPkg,exception);
					
					cout<<"Image received"<<endl;
					
					strcpy(Img->filename,filename.c_str());
					WriteImage(imageinfo,Img);
					
					break;					
				
					
					
				default:
					cout<<"Bad package type : "<<c<<endl;
			}
		}
	}
	
	return 0;
}




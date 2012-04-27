#include <boost/asio.hpp> 
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

void receiveFile(const char * filename,long length,boost::asio::serial_port port)
{
	ofstream myFile(filename, ios::out | ios::binary);
	char c;
	if (myFile)
	{
		for (long k;k<length;k++)
		{
			read(port,boost::asio::buffer(&c,1));
			myFile.write(c);
		}
	}
	else
	{
		cout<<"Error : Unable to open "<<filename<<endl;
	}
}


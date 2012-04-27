#include <iostream>
#include <stdlib.h>
#include "TimeoutSerial.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		cout<<"Error: not enough arguments"<<endl;
		return 1;
	}
	try
	{
		char * pEnd;
		char * data;
		TimeoutSerial serial(argv[1],strtol(argv[2],&pEnd,10));
		serial.setTimeout(posix_time::seconds(5));
		
		serial.writeString("Hello world\r\n");
		serial.read(data,3);
		cout<<data<<endl;
		return 0;
	}
	catch(boost::system::system_error& e)
	{
		cout<<"Error: "<<e.what()<<endl;
		return 1;
	}
}


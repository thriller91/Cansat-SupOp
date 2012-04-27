#include <SerialStream.h>
#include <iostream>

using namespace LibSerial;
using namespace std;

SerialStream serial_stream;

int main(int argc,char** argv)
{
	char response[3];

	serial_stream.Open( argv[1] ) ;
	serial_stream.SetBaudRate( SerialStreamBuf::BAUD_115200 ) ;
	serial_stream.SetCharSize( SerialStreamBuf::CHAR_SIZE_8 ) ;
	serial_stream.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE ) ;
	serial_stream.SetParity( SerialStreamBuf::PARITY_NONE ) ;
	serial_stream.SetNumOfStopBits(0) ;
	
	while (1)
	{
		serial_stream<<"ABCD";
		cout<<"waiting response"<<endl;
		serial_stream.read(response,3);
		cout<<response<<endl;
	}
	return 0;
}


all:
	
	g++ -O2 -c comm2.cpp
	g++ -O2 -c TimeoutSerial.cpp
	g++ -o comm2 comm2.o TimeoutSerial.o -lboost_system
	
clean:
	rm comm2.o TimeoutSerial.o

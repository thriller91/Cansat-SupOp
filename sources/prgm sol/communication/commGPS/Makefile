all:
	
	g++ -c main.cpp -g
	g++ -c commGPS.cpp -g
	g++ -o commGPS main.o commGPS.o TinyGPS.o -lboost_system -g
	rm main.o commGPS.o
	
clean:
	rm commGPS

all:
	
	g++ -O2 -c main.cpp
	g++ -O2 -c comm4.cpp
	g++ -o comm4 main.o comm4.o -lboost_system
	rm main.o comm4.o
	
clean:
	rm comm4

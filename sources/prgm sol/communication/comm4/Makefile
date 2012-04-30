all:
	
	g++ -c main.cpp -g
	g++ -c comm4.cpp -g
	g++ -o comm4 main.o comm4.o -lboost_system -g
	rm main.o comm4.o
	
clean:
	rm comm4

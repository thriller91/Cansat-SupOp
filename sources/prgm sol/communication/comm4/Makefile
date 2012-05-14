all:
	
	g++ -c main.cpp -g `pkg-config --cflags --libs MagickCore`
	g++ -c comm4.cpp -g
	g++ -o comm4 main.o comm4.o -lboost_system `pkg-config --cflags --libs MagickCore` -g
	rm main.o comm4.o
	
clean:
	rm comm4

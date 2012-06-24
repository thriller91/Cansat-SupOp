CC=g++
CFLAGS=-W -Wall
LDFLAGS=-I/usr/include/boost -lboost_system

all: camera

camera: main.o
	$(CC) -o camera main.o $(LDFLAGS)
	
main.o: main.cpp
	$(CC) -o main.o -c main.cpp $(CFLAGS)
	
clean:
	rm *.o


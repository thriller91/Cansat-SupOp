CC=g++
CFLAGS=-W -Wall
LDFLAGS=-I/usr/include/boost -lboost_system


all: comm

comm: main.o
	
	$(CC) -o comm main.cpp $(LDFLAGS)

main.o: main.cpp
	$(CC) -o main.o -c main.cpp $(CFLAGS)
	
clean:
	rm *.o

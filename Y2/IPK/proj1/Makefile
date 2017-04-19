CC=g++

all: ftrestd ftrest

ftrestd: server.cpp
	$(CC) -Wall -std=c++11 -o $@ server.cpp

ftrest: client.cpp
	$(CC) -Wall -std=c++11 -o $@ client.cpp

clean:
	rm ftrestd ftrest

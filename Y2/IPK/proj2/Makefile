# AUTHOR: xbazik00
# Projekt: ipk-client
# Date: 11.04.2017

CC= g++
CFLAGS= -Wall -lm -std=c++11
LDFLAGS= -lcrypto -lssl

all: ipk-client

inpk-client: ipk-client.cpp
	$(CC) $(CFLAGS) $@ -o $< $(LDFLAGS)
clean:
	rm -rf ipk-client 
	rm -rf xbazik00.tgz

zip: ipk-client.cpp ipk-client.1 Makefile
	tar -cvf xbazik00.tgz $^ 

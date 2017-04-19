# primes.c 
# Řešení IJC-DU1, příklad a),b), 7.5.2016 
# Autor: Martin Bažík, FIT 
# Přeloženo: gcc 4.9.2
# Popis: Makefile.

CC=gcc

CFLAGS = -O2 -std=c99 -Wall -pedantic -lm -g
all: primes primes-inline steg-decode

primes.o: primes.c error.h bit_array.h eratosthenes.h
	$(CC) $(CFLAGS) -c primes.c -o primes.o
eratosthenes.o: eratosthenes.c eratosthenes.h error.h bit_array.h
	$(CC) $(CFLAGS) -c eratosthenes.c -o eratosthenes.o
error.o: error.c error.h 
	$(CC) $(CFLAGS) -c error.c -o error.o
ppm.o: ppm.c ppm.h error.h eratosthenes.h bit_array.h
	$(CC) $(CFLAGS) -c ppm.c -o ppm.o
steg-decode.o: steg-decode.c eratosthenes.h error.h ppm.h bit_array.h
	$(CC) $(CFLAGS) -c steg-decode.c -o steg-decode.o

primes: primes.o eratosthenes.o error.o  
	$(CC) $(CFLAGS) -o primes primes.o eratosthenes.o error.o 
primes-inline: primes.c eratosthenes.c error.o  
	$(CC) $(CFLAGS) -o primes-inline -DUSE_INLINE primes.c eratosthenes.c error.o 
steg-decode: steg-decode.o eratosthenes.o error.o ppm.o 
	$(CC) $(CFLAGS) -o steg-decode steg-decode.o eratosthenes.o error.o ppm.o 

clean: 
	rm -f *.o 
	rm -f primes
	rm -f primes-inline 
	rm -f steg-decode

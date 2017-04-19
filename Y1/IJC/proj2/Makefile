CC=gcc
CP=g++

CFLAGS= -O2 -std=c99 -Wall -pedantic -g
CPPFLAGS=-O2 -std=c++11 -Wall -pedantic -g
LBFILES=htab_init.o htab_init2.o htab_foreach.o htab_lookup_add.o hash_function.o htab_remove.o htab_clear.o htab_free.o

all: wordcount wordcount-dynamic tail tail2


wordcount.o: main.c htable.h
	$(CC) $(CFLAGS) -c main.c -o wordcount.o
hash_function.o: hash_function.c hash_function.h
	$(CC) $(CFLAGS) -fPIC -c hash_function.c
htab_init.o: htab_init.c htable.h
	$(CC) $(CFLAGS) -fPIC -c htab_init.c -o htab_init.o
htab_init2.o: htab_init2.c htable.h
	$(CC) $(CFLAGS) -fPIC -c htab_init2.c -o htab_init2.o
htab_lookup_add.o: htab_lookup_add.c htable.h
	$(CC) $(CFLAGS) -fPIC -c htab_lookup_add.c -o htab_lookup_add.o
htab_foreach.o: htab_foreach.c htable.h
	$(CC) $(CFLAGS) -fPIC -c htab_foreach.c -o htab_foreach.o	
htab_remove.o: htab_remove.c htable.h
	$(CC) $(CFLAGS) -fPIC -c htab_remove.c -o htab_remove.o	
htab_clear.o: htab_clear.c htable.h
	$(CC) $(CFLAGS) -fPIC -c htab_clear.c -o htab_clear.o	
htab_free.o: htab_free.c htable.h
	$(CC) $(CFLAGS) -fPIC -c htab_free.c -o htab_free.o	
io.o: io.c io.h
	$(CC) $(CFLAGS) -c io.c -o io.o


libhtable.a: $(LBFILES)
	ar crs libhtable.a $(LBFILES)

libhtable.so: $(LBFILES)
	$(CC) $(CFLAGS) -shared -fPIC -o libhtable.so $(LBFILES) 


wordcount: wordcount.o io.o libhtable.a  
	$(CC) $(CFLAGS) -o wordcount -static wordcount.o io.o libhtable.a 
wordcount-dynamic: wordcount.o io.o libhtable.so  
	$(CC) $(CFLAGS) -o wordcount-dynamic wordcount.o io.o libhtable.so

tail: tail.c
	$(CC) $(CFLAGS) -o tail tail.c
tail2: tail2.cc
	$(CP) $(CPPFLAGS) -o tail2 tail2.cc

clean:
	rm -rf $(LBFILES) wordcount wordcount-dynamic wordcount.o io.o tail.o tail2.o tail tail2


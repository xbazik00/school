# Projekt ISA
# Author: Martin Bazik, xbazik00
# Date: 18.11.2017

CC= g++
CFLAGS= -std=c++11 -Wall
OBJ= server.o ldapop.o ldapmessage.o ldap.o

all: myldap

myldap: $(OBJ) 
	$(CC) $(CFLAGS) -o $@ $^

server.o: server.cpp ldap.h ldapmessage.h ldapop.h
	$(CC) $(CFLAGS) -o $@ -c $<

ldapop.o: ldapop.cpp ldapop.h
	$(CC) $(CFLAGS) -o $@ -c $<

ldapmessage.o: ldapmessage.cpp ldapmessage.h ldapop.h
	$(CC) $(CFLAGS) -o $@ -c $<

ldap.o: ldap.cpp ldap.h ldapmessage.h ldapop.h
	$(CC) $(CFLAGS) -o $@ -c $<

tar:
	cp -f doc/manual.pdf .;\
	tar cf xbazik00.tar ldap*.cpp ldap*.h server.cpp Makefile README manual.pdf 

clean:
	rm -f myldap $(OBJ) xbazik00.tar manual.pdf

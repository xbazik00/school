# LDAP server

This project implements LDAP server. It is a concurrent server that works on the protocol LDAPv2 and carries out Search operations. It implements basic search filters including AND, OR, NOT, SUBSTRING, EQUALITY_MATCH and PRESENT. 

## Getting Started

Follow the instructions to get the project running. These instructions will walk you through compilation and running process.

### Prerequisites

* compiler supporting c++ 11


### Compiling

Makefile possibilities:

```
make
make myldap     --  builds program
make clean      --  to remove object files and binaries
make tar        --  packes the project into .tar file 
```

### Running the program

After you have build the program yo can run it with the following parameters.

```
./myldap {-p <port>} -f <file>

Parameters explained:

    -p <port>: Allows you to choose number of the port the server will listen on.  Default value is 389.
    -f <file>: Path to the CSV file containing data. 
```

## Source files

* ldap.cpp --> LDAP class, LDAP communication
* ldapmessage.cpp --> LDAPMessage class, LDAP message envelope
* ldapop.cpp --> LDAPProtocolOp class, LDAP operations
* server.cpp --> main function, BSD socket concurrent communication

## Header files
* ldap.h --> ldap.cpp
* ldapmessage.h --> ldapmessage.cpp
* ldapop.h --> ldapop.cpp

## Documentation files
* manual.pdf --> Documentation in Slovak language
* README --> basic README file

## Other files
* Makefile --> makefile for the project

## CSV data file format
Data must be in following format.
```
cn;uid;mail;[\r\n]
```
## Expansions
* The server can work with national characters in UTF-8 encoding.
* The server will return given attributes when asked. Example:
    ```
    ldapsearch -h hostname -p port -x -s base "(filter)" uid
    ```
    * This will return only __uid__ attribute for each object.
* The server will handle PRESENT filter as well.


## Bugs
* When server receives an invalid request of unrecognized operation and closes the socket, it remains in TIME_WAIT state after the program is terminated.
* This may and may not be a bug.

## Author

* **Martin Bazik** - *xbazik00@stud.fit.vutbr.cz*



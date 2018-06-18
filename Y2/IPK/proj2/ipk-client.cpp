/**
 * Client for mathematical operations.
 * IPK Project 2
 *
 * @file ipk-client.cpp
 * @brief Client for file transfer application.
 *
 * @author Martin Bazik
 * @version 1.0 11/4/17
 */

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <grp.h>
#include <openssl/md5.h>
#include <arpa/inet.h>
#include <limits.h>
#include <math.h>


#define ERROR (-1)
//#define DEFAULT_PORT (6677)
#define DEFAULT_PORT (55555)
#define BUF_SIZE (1)

using namespace std;


/**
 * Establishes connection with server.
 *
 * @param	host	host name to connect
 * @param	port	port to connect
 * @param	soc	returns socket used
 * 
 * @return	ERROR in case of error, else 0 	
 */
int establish_connection(string host, int port, int* soc){
	struct sockaddr_in server_v4;
	struct sockaddr_in6 server_v6;


	if(inet_aton(host.c_str(), &server_v4.sin_addr) != 0){
		// Creates socket
		if((*soc = socket(PF_INET, SOCK_STREAM, 0)) < 0){
			return ERROR;
		}
		
		// Sets up socket
		server_v4.sin_family = PF_INET;
		server_v4.sin_port = htons(port);

		// Connects to socket
		if(connect(*soc,(struct sockaddr *) &server_v4, sizeof(server_v4)) < 0){
			return ERROR;
		}
	}
	else if(inet_pton(AF_INET6,host.c_str(),&server_v6.sin6_addr.s6_addr) != 0){
		// Creates socket
		if((*soc = socket(PF_INET6, SOCK_STREAM, 0)) < 0){
			return ERROR;
		}
		// Sets up socket
		server_v6.sin6_family = PF_INET6;
		server_v6.sin6_port = htons(port);

		// Connects to socket
		if(connect(*soc,(struct sockaddr *) &server_v6, sizeof(server_v6)) < 0){
			return ERROR;
		}
	}
	else{
		return ERROR;
	}

	return 0;
}

/**
 * Creates HELLO request containing MD5 hash of login string
 *
 * @param hello returns request by reference
 *
 * @return 0 if correct
 */
void create_hello(string *hello){
	*hello = "HELLO ";
	unsigned char digest[MD5_DIGEST_LENGTH];
	char string[] = "xbazik00";
    
	MD5((unsigned char*)&string, strlen(string), (unsigned char*)&digest);    
 
	char mdString[33];
 
	for(int i = 0; i < 16; i++)
        	sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

	*hello += mdString;
	*hello += "\n";
}

/**
 * Processes expression given to be solved by the server. It calculates
 * the answer of the equation and returns it.
 *
 * @param exp mathematical expression
 * @return result of the expression
 */
string process_exp(string exp){
	int pos = 0;		// position in string
	string number = string();	// temporary string for number
	string op = string();	// string for operator
	double first = 0;	// first operand
	double second = 0;	// second operand
	char *end_ptr;		// end pointers for conversion
	char *end_ptr2;
	string result = string();
	double number_result = 0;

	// First operand
	if((pos = exp.find(" ")) != string::npos){
		number = exp.substr(0,pos);
		exp = exp.substr(pos+1);
		first = strtoll(number.c_str(),&end_ptr,0);
		int err = errno;
		
		if(*end_ptr != '\0'){	// not a number
			return "ERROR2";
		}
		
		if(first == LLONG_MAX || first == LLONG_MIN){	// overflow
			return "ERROR";
		}

	}
	else{
		return "ERROR2";
	}

	// Operation
	if((pos = exp.find(" ")) != string::npos){
		op = exp.substr(0,pos);
		exp = exp.substr(pos+1);	
	}
	else{
		return "ERROR2";
	}

	// Second operand
	if((pos = exp.find("\n")) != string::npos){
		number = exp.substr(0,pos);
		second = strtoll(number.c_str(),&end_ptr2,0);
		if(*end_ptr2 != '\0'){	// not a number
			return "ERROR2";
		}

		if(second == LLONG_MAX || second == LLONG_MIN){	// overflow
			return "ERROR";
		}
	}
	else{
		return "ERROR2";
	}

	// Calculation of result
	if(op == "*"){
		number_result = first * second;
	}
	else if(op == "+"){
		number_result = first + second;
	}
	else if(op == "-"){
		number_result = first - second;
	}
	else if(op == "/"){
		if(second == 0){	// Division by zero
			return "ERROR";
		}
		number_result = first / second;
	}
	else{
		return "ERROR2";
	}

	number_result = trunc(number_result * 100)/100;

	stringstream result_stream;

	result_stream.setf(ios::fixed);
	result_stream.setf(ios::showpoint);

	result_stream << number_result;
	result = result_stream.str();

	// Two decimal places
	pos = result.find(".");
	result = result.substr(0,pos+3);

	return result;
}

/**
 * Main function
 */
int main(int argc, char* argv[]){
	int socket;	
	int port = DEFAULT_PORT;	// Number of port
	string host = "localhost";	// Name of the host
	string hello;

	// Parses parameters
	if(argc == 2){
		host = argv[1];
	}
	else{
		cerr << "Error: Invalid combination of parameters." << endl;
		return 1;
	}
	
	if(establish_connection(host,port,&socket) == ERROR){
		cerr << "Error: Connection was not established." << endl;
		return 2;
	}

	create_hello(&hello);

	if(send(socket, hello.c_str(), hello.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 3;
	}
	
	char buffer[BUF_SIZE];
	int n = 0;
	string request = string();
	int count = 0;
	while(1){
		// Receives request
		memset(buffer,0,BUF_SIZE);	
		while ((n = recv(socket, buffer, BUF_SIZE, 0)) > 0){	
			request.append(buffer,n);
			if(n < BUF_SIZE - 1){
				break;
			}

			if((buffer[0] == '\n') or (buffer[0] == '\0')){
				break;
			}		
			memset(buffer,0,BUF_SIZE);
		}
		//cout << count++ << request;

		string secret = string();
		int pos = 0;

		/*
		 * Final request BYE ends connection and client prints out
		 * secret.
		 */
		if((pos = request.find("BYE ")) == 0){
			request = request.substr(pos + 4);
			if(((pos = request.find("\n")) != string::npos)){
				secret = request.substr(0,pos);
			}
			if(secret.find(" ") == string::npos){
				cout << secret;
				break;
			}
		}

		/*
		 * The request is SOLVE and result is calculated.
		 */
		else if(request.find("SOLVE ") == 0){
			string res;
			if((res = process_exp(request.substr(6))) == "ERROR2"){
				request = "";
				continue;
			}
			string result = "RESULT " + res + "\n";
			//cout << result;
			if(send(socket, result.c_str(), result.size(), 0) < 0){
				cerr << "Error: Request was not sent." << result << endl;
				return 3;
			}
			request = "";
		}


		request = "";
	}

	close(socket);	

	return 0;
}

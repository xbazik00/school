/**
 * Server for HTTP protocol using RESTful API.
 * IPK Project 1
 *
 * @file server.cpp
 * @brief Server for file transfer application.
 *
 * @author Martin Bazik
 * @version 1.0 15/3/17
 */


// TODO: rozsirenia - password, gzip/bz2


#include <ctime>
#include <iostream>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <sstream>
#include <fstream>
#include <regex.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>

#define NUMBER_CLIENTS (5)
#define ERROR (-1)
#define DEFAULT_PORT (6677)
#define BUF_SIZE (2048)
#define MAX_PATH (1024)
#define MIN_PORT (1024)
#define MAX_PORT (65535)
#define TIME_BUF (80)

using namespace std;

/**
 * Establishes connection with client.
 *
 * @param	port	port to connect
 * @param	soc	returns socket used
 * @param	server	returns received socket address
 *
 * @return	ERROR in case of error, else 0  
 */
int establish_connection(int port, int *soc, struct sockaddr_in6 *server){
	if((*soc = socket(PF_INET6, SOCK_STREAM, 0)) < 0){
		return ERROR;
	}

	server->sin6_family = PF_INET6;
	server->sin6_port = htons(port);
	server->sin6_addr = in6addr_any;

	if(bind(*soc, (struct sockaddr *)server, sizeof(*server)) < 0){
		return ERROR;
	}

	if(listen(*soc, NUMBER_CLIENTS)){
		return ERROR;
	}
	return 0;
}

/**
 * Checks if username is used.
 *
 * @param	path		root directory
 * @param	username	searched username
 *
 * @return	true if it is present, else false
 */
bool correct_user(string path, string username){
	struct dirent *user_entity;
	DIR *user_obj;
	user_obj = opendir(path.c_str());
	if(user_obj != NULL){
		while ((user_entity = readdir(user_obj)) != NULL) {
			if(username.compare(user_entity->d_name) == 0){
				return true;
			}
		}
	}
	return false;
}

/**
 * Creates JSON element given key and value
 *
 * @param	key	JSON key
 * @param	value	JSON value
 *
 * @return	ERROR in case of error, else 0  
 */
string make_json_element(string key, string value){
	string json_el;
	json_el += string(" \"");
	json_el += key;
	json_el += string("\":\""); 
	json_el += value;
	json_el += string("\",");
	return json_el;

}

/**
 * Communicates with the client specified in "acc_soc" socket. Firstly it parses
 * the given request and sends response to this request to this client. 
 *
 * @param	acc_soc		socket for
 * @param	path		received socket address
 *
 * @return	ERROR in case of error, else 0  
 */
int communicate(int acc_soc,string path){
	string answer;		// Generated answer
	string answer_return;	// error code and error string
	string file_path;
	string file_dir;
	string answer_body;
	string answer_type;
	string size;
	string username;
	string sent_type;

	char buffer[BUF_SIZE];
	int n = 0;
	
	// Receives request
	memset(buffer,0,BUF_SIZE);	
	string request = string();

	while ((n = recv(acc_soc, buffer, BUF_SIZE, 0)) > 0){
		request.append(buffer,n);
		if(n < BUF_SIZE - 1){
			break;
		}
	}
	n = 0;
	memset(buffer,0,BUF_SIZE);
	cout << request;
	request = "";
	


	string hello = "SOLVE 102222222222222222222222222222 / 3\n";
	if(send(acc_soc, hello.c_str(), hello.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}

	while ((n = recv(acc_soc, buffer, BUF_SIZE, 0)) > 0){
		request.append(buffer,n);
		if(n < BUF_SIZE - 1){
			break;
		}
	}

	n = 0;
	memset(buffer,0,BUF_SIZE);
	cout << request;
	request = "";

	hello = "SOLVE10 / 3\n";
	if(send(acc_soc, hello.c_str(), hello.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}

	n = 0;
	memset(buffer,0,BUF_SIZE);
	cout << request;
	request = "";

	hello = "SOLVE -54564654 + 545d\n";
	if(send(acc_soc, hello.c_str(), hello.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}
	/*while ((n = recv(acc_soc, buffer, BUF_SIZE, 0)) > 0){
		request.append(buffer,n);
		if(n < BUF_SIZE - 1){
			break;
		}
	}*/
	n = 0;
	memset(buffer,0,BUF_SIZE);
	cout << request;
	request = "";

	hello = "D";
	if(send(acc_soc, hello.c_str(), hello.size()+1, 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}
	n = 0;
	memset(buffer,0,BUF_SIZE);
	cout << request;
	request = "";

	hello = "D\n";
	if(send(acc_soc, hello.c_str(), hello.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}
	n = 0;
	memset(buffer,0,BUF_SIZE);
	cout << request;
	request = "";

	hello = "SOLVE 8 - 9\n";
	if(send(acc_soc, hello.c_str(), hello.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}

	while ((n = recv(acc_soc, buffer, BUF_SIZE, 0)) > 0){
		request.append(buffer,n);
		if(n < BUF_SIZE - 1){
			break;
		}
	}
	n = 0;
	memset(buffer,0,BUF_SIZE);
	cout << request;
	request = "";

/*	if(send(acc_soc, hello.c_str(), hello.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}
*/

	string bye = "BYE d123w123\n";
	if(send(acc_soc, bye.c_str(), bye.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}
	exit(0);


	/*
	// Separates header and body of request
	int pos = request.find("\r\n\r\n");
	
	string head = request.substr(0,pos);
	string body = request.substr(pos+4);
	
	// Parses header of a request
	int pos_b = 0;
	int pos_e = head.find(" ");
	string command = head.substr(pos_b,pos_e);

	pos_b = pos_e + 2;
	pos_e = head.find("?type=");
	string user_path = head.substr(pos_b,pos_e-pos_b);	
	

	int space_sep = 0;
	while((size_t)(space_sep = user_path.find("%20")) != string::npos){
		user_path.replace(space_sep,3," ");
	}

	username += user_path.substr(0,user_path.find("/"));

	pos_b = pos_e + 6;
	pos_e = head.find(" ",pos_b);
	string type = head.substr(pos_b,pos_e-pos_b);

	if((size_t)(pos_b = head.find("Content-Type: "))!= string::npos){
		pos_e = head.find("\r\n",pos_b); 
		sent_type = head.substr(pos_b + 14,pos_e-pos_b - 14);
	}

*/
	return 0;

}

/**
 * Returns current working directory. In case of error
 * it throws an exception.
 *
 * @return	current working directory
 */
string get_cwd(){
	char path[MAX_PATH];

	if (getcwd(path, MAX_PATH) != 0) 
		return string (path);

	int error = errno;
	switch(errno) {
		case EACCES:
			throw std::runtime_error("Access denied");

		case ENOMEM:
			throw std::runtime_error("Insufficient storage");

		default:
			ostringstream str;
			str << "Unrecognised error" << error;
			throw std::runtime_error(str.str());
	}
}

/**
 * Main function 
 *
 */
int main(int argc, char* argv[]){
	string root_path = string(get_cwd());	// root directory is CWD by default
	string str_port = string();
	struct stat dir_stat;
	int c;
	int port = DEFAULT_PORT;
	
	// Parsing parameters using getopt
	while ((c = getopt (argc, argv, "r:p:")) != -1){
		switch (c){
			case 'r':
				root_path = string(optarg);
				if (!(stat(root_path.c_str(), &dir_stat) == 0 && S_ISDIR(dir_stat.st_mode))){
					cerr << "Invalid root path." << endl;
					return 1;
				}
				break;
			case 'p':
				str_port = string(optarg);
				if(!isdigit(str_port[0])){
					cerr << "Port is not a number." << endl;
					return 1;
				}
				char *p ;
				port = strtol(str_port.c_str(), &p, 10);
				if(*p != 0){
					cerr << "Port is not a number." << endl;
					return 1;
				}
				if(port < MIN_PORT || port > MAX_PORT){
					cerr << "Port is not within available ports." << endl;
					return 1;
				}
				break;
			default:
				cerr << "Unexpected parameters" << endl;
				exit(1);
		}
	}

	// socket variables
	struct sockaddr_in6 server;
	int socket;
	socklen_t soc_len;
	int acc_soc;

	// starts connection
	if(establish_connection(port,&socket,&server) == ERROR){
		return 1;
	}

	soc_len = sizeof(server);
	
	// Infinite loop to keep server running
	while (1){
		// Accepts incoming socket
		if((acc_soc = accept(socket, (struct sockaddr *) &server, &soc_len)) < 0){
			return 1;
		}

		pid_t pid = fork();
		if(pid == 0){

			// Chat initialised
			if(communicate(acc_soc,root_path) == ERROR){
				return 1;
			}
			break;
		}

		// Error while forking
		else if(pid < 0){
			cerr << "Error: Could not create process." << endl;
			return 1;
		}
		
	}
	close(socket);

	return 0;
}

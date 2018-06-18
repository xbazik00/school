/**
 * Server for LDAP protocol.
 * ISA Project
 *
 * @file server.cpp
 * @brief Server for LDAP communication.
 *
 * @author Martin Bazik, xbazik00
 * @version 1.0 12/11/17
 */

#include <netinet/in.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

#include "ldap.h" // my library

#define ERROR (-1)
#define MAX_PATH (1024)
#define NUMBER_CLIENTS (5)
#define DEFAULT_PORT (389)
#define MIN_PORT (1)
#define MAX_PORT (65535)


using namespace std;


/**
 * Sets up socket.
 *
 * @param	port	port to connect
 * @param	soc	returns socket used
 * @param	server	returns received socket address
 *
 * @return	ERROR in case of error, else 0  
 */
int setup_socket(int port, int *soc, struct sockaddr_in *server){
	memset(server,0,sizeof(*server));

	// creates socket IPv4 TCP socket
	if((*soc = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		return ERROR;
	}

	server->sin_family = AF_INET;
	server->sin_port = htons(port);
	server->sin_addr.s_addr = INADDR_ANY;

	if(bind(*soc, (struct sockaddr *)server, sizeof(*server)) < 0){
		return ERROR;
	}

	if(listen(*soc, NUMBER_CLIENTS)){
		return ERROR;
	}
	return 0;
}

/**
 * Main function for LDAP server
 * 
 * @param	argc	argument count
 * @param	argv	arguments
 *
 * @return	1 in case of error, else 0  
 */
int main(int argc, char* argv[]){
	string csv_path;	// CSV file with object database
	string str_port = string();	// Port string for argument parsing 

	struct stat file_stat; // File stats
	
	int c;	// Option from getopts
	int port = DEFAULT_PORT;	// Port number
	size_t p = 0;	// Variable for checking port conversion length
	bool file_used = false;	// Was file defined?

	// Parsing parameters using getopt
	while ((c = getopt (argc, argv, "p:f:")) != -1){
		switch (c){
			case 'f':	// file
				csv_path = string(optarg);
				if (!(stat(csv_path.c_str(), &file_stat) == 0 && S_ISREG(file_stat.st_mode))){
					cerr << "Invalid CSV file path." << endl;
					return 1;
				}
				file_used = true;
				break;
			case 'p':	// port
				str_port = string(optarg);
				p = 0;
				try{
					port = stol(str_port,&p,10);	// conversion
					if(p != str_port.length()){
						cerr << "Port is not a number." << endl;
						return 1;
					}
				}
				catch (exception& e){
					cerr << "Invalid port number." << endl;
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

	// No file parameter
	if(!file_used){
		cerr << "Parameter -f is not used!" << endl;
		exit(1);
	}

	// socket variables
	struct sockaddr_in server;
	struct sockaddr_in client;

	int socket;
	socklen_t soc_len;
	int acc_soc;	// client connected socket
	ifstream file;
	
	// sets up socket
	if(setup_socket(port,&socket,&server) == ERROR){
		return 1;
	}

	soc_len = sizeof(client);
	
	// Infinite loop to keep server running
	while (1){
		// Accepts incoming socket
		if((acc_soc = accept(socket, (struct sockaddr *) &client, &soc_len)) < 0){
			cerr << "Error: Could not accept connection" << endl;
			return 1;
		}		

		pid_t pid = fork();

		if(pid < 0){
			cerr << "Error: Could not create process." << endl;
			return 1;
		}

		if(pid == 0){
			// child process
			close(socket);
			// opens file
			file.open(csv_path);
			if(!file.is_open()){
				exit(1);
			}

			// instance for LDAP
			LDAP ldap;

			// LDAP communication
			if(ldap.communicate(acc_soc,file) == ERROR){
				return 1;
			}
			
			cout << "Closing file" << endl;
			file.close();
			
			exit(0);
		}

		// parent process
		else {
			close(acc_soc);
		}

		
	}
	close(socket);
	return 0;
}

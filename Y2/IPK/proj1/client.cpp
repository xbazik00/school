/**
 * Client for HTTP protocol using RESTful API.
 * IPK Project 1
 *
 * @file client.cpp
 * @brief Client for file transfer application.
 *
 * @author Martin Bazik
 * @version 1.0 15/3/17
 */

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
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h> 
#include <grp.h>

#define ERROR (-1)
#define DEFAULT_PORT (6677)
#define MAX_PATH (1024)
#define BUF_SIZE (2048)
#define TIME_BUF (80)

// Invididual operations
enum command_t {NONE,GET,PUT,LST,MKD,RMD,DEL,MV,CP,LSTMOR};

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
	struct sockaddr_in server;
	struct hostent *hptr;

	// Creates socket	
	if((*soc = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		return ERROR;
	}
	
	// Sets up socket
	server.sin_family = PF_INET;
	server.sin_port = htons(port);
	
	// Host identified by name
	if((hptr = gethostbyname(host.c_str())) == NULL){
		return ERROR;
	}

	// Sets up server address
	memcpy(&server.sin_addr, hptr->h_addr, hptr->h_length);

	// Connects to socket
	if(connect(*soc,(struct sockaddr *) &server, sizeof(server)) < 0){
		return ERROR;
	}
	return 0;
}

/**
 * Parses path of file on a remote server.
 *
 * @param	path_svr	URI of remote server
 * @param	port		returns used port
 * @param	svr_name	returns name of a server (IP)
 * @param	rest		returns rest of parsed
 * 
 * @return	ERROR in case of error, else 0 	
 */
int parse_remote_path(string path_svr, int *port, string *svr_name, string *rest){

	

	// URI corect?
	if(path_svr.find(string("http://")) != 0){
		cerr << "Invalid REMOTE_PATH (no http://)" << endl;
		return ERROR;
	}

	// Erases "http://"
	path_svr.erase(0,7);

	// Parses server name
	int name_end = path_svr.find(string("/"));
	*svr_name = path_svr.substr(0,name_end);

	// Parses server port
	int port_pos = svr_name->find(string(":")); 
	string port_s = svr_name->substr(port_pos + 1); 
	if((size_t)port_pos != string::npos){
		if(!isdigit(port_s[0])){
			cerr << "Port is not a number." << endl;
			return ERROR;
		}
		char *p ;
		*port = strtol(port_s.c_str(), &p, 10);
		if(*p != 0){
			cerr << "Port is not a number." << endl;
			return ERROR;
		}

		svr_name->erase(svr_name->begin() + port_pos,svr_name->end());
	}

	// Rest of URI
	*rest = path_svr.substr(name_end + 1);
	return 0;
}

/**
 * Creates HTTP header for indvidual commands. The header is compatible
 * with HTTP 1.1
 *
 * @param	head		returns header
 * @param	path		path on remote server
 * @param	host		host name
 * @param	type		type of command
 * @param	file_size	size of a file
 * 
 * @return	ERROR in case of error, else 0 	
 */
int create_HTTP_header(string *head, string path, string host, command_t type, int file_size){
	string command;
	string fod;	// file or directory
	string accept = "application/json";
	string json;

	switch(type){
		case CP:
		case MV:		
		case PUT:
			command = string("PUT");
			fod = string("file");
			break;
		case GET:
			command = string("GET");
			fod = string("file");
			accept = "application/octet-stream";
			break;
		case DEL:
			command = string("DELETE");
			fod = string("file");
			break;
		case LSTMOR:
		case LST:
			command = string("GET");
			fod = string("folder");
			break;
		case MKD:
			command = string("PUT");
			fod = string("folder");
			break;
		case RMD:
			command = string("DELETE");
			fod = string("folder");
			break;
		default:
			return ERROR;
	}

	// Sets up time
	time_t t = time(0);
	struct tm *now = localtime(&t);
  	char buffer[TIME_BUF];

	strftime(buffer,TIME_BUF,"%a, %d %b %G %T %Z",now);

	if(path.back() != '/' && type != PUT && type != MV && type != CP){
		path += string("/");
	}

	// Substitution for spaces in uri
	int space_sep = 0;
	while((size_t)(space_sep = path.find(" ")) != string::npos){
		path.replace(space_sep,1,"%20");
	}
	// Header
	*head = command + string(" /") + path + string("?type=") + fod + string(" HTTP/1.1\r\n"); 
	*head += string("Host: ") + host + string("\r\n");
	*head += string("Date: ") + string(buffer) + string("\r\n");
	*head += string("Accept: ") + accept + string("\r\n");
	*head += string("Accept-Encoding: identity");
	if(type == PUT){
		*head += string("\r\nContent-Type: application/octet-stream\r\n");
		*head += string("Content-Length: ") + to_string(file_size);
	}
	else if(type == MV || type == CP || type == LSTMOR){
		*head += string("\r\nContent-Type: application/json\r\n");
		*head += string("Content-Length: ") + to_string(file_size);
	}
	*head += string("\r\n\r\n");
	return 0;
}


/**
 * Returns Current Working Directory.
 * 
 * @return	cwd or NULL in cse of error
 */
string get_cwd(){
	char path[MAX_PATH];

	if (getcwd(path, MAX_PATH) != 0) 
		return string (path);
	return NULL;
}

/**
 * Converts file to string. 
 *
 * @param	path	path of converted file
 * @param	file	returns file in string
 * 
 * @return	ERROR in case of error, else 0 	
 */
int get_file(string path, string *file){
	ifstream fin(path, ios::binary);
	if(!fin.is_open()){
		return ERROR;
	}
	ostringstream ostrm;
	ostrm << fin.rdbuf();
	*file = ostrm.str();
	fin.close();
	return 0;
}

/**
 * Reads JSON status.
 *
 * @param	json	jason response body
 * @param	status	returns status of response
 */
void read_status(string body, string *status){
	int start = body.find(":\"") + 2;
	int end = body.rfind("\"");
	*status = body.substr(start,end-start);
}

/**
 * Writes error at the standard error output. Depending on status of response.
 *
 * @param	status	status of response
 */
void write_err(string status){
	if(status.compare("not_file") == 0){
		cerr << "Not a file." << endl;
	}
	else if(status.compare("file_not_found") == 0){
		cerr << "File not found." << endl;
	}
	else if(status.compare("user_not_found") == 0){
		cerr << "User Account Not Found" << endl;
	}
	else if(status.compare("not_dir") == 0){
		cerr << "Not a directory." << endl;
	}
	else if(status.compare("dir_not_found") == 0){
		cerr << "Directory not found." << endl;
	}
	else if(status.compare("missing_permission") == 0){
		cerr << "Unknown error." << endl;
	}
	else if(status.compare("unavailable_dir") == 0){
		cerr << "Unknown error." << endl;
	}
	else if(status.compare("not_empty") == 0){
		cerr << "Directory not empty." << endl;
	}
	else if(status.compare("user") == 0){
		cerr << "Unknown error." << endl;
	}
	else if(status.compare("already_exists") == 0){
		cerr << "Already exists." << endl;
	}
	else if(status.compare("unknown_error")){
		cerr << "Unknown error." << endl;
	}
	else{
		cerr << "Unknown error." << endl;
	}
}

/**
 * Parses body of lst response in case of extension.
 *
 * @param	body	body of a response
 *
 * @return	string containing output of this command
 */
string parse_lst(string body){
	string output;
	int count = 0;
	int start;
	int end;

	// Iterates and parses individual properties
	while((size_t)(body.find(":")) != string::npos){

		// Name
		string elem = string("\"element") + to_string(count) + string("\":\"");
		start = body.find(elem) + elem.length();
		end = body.find("\"",start);
		string name= body.substr(start,end-start);
	
		// File size
		string size = string("\"size") + to_string(count) + string("\":\"");
		start = body.find(size) + size.length();
		end = body.find("\"",start);
		string sz= body.substr(start,end-start);
		
		// file type (d/-)
		string type = string("\"type") + to_string(count) + string("\":\"");
		start = body.find(type) + type.length();
		end = body.find("\"",start);
		string dir = body.substr(start,end-start);
		string d;
		if(!dir.compare("dir")){
			d = string("d");
		}
		else{
			d = string("-");
		}

		// permissions
		string perm = string("\"perm") + to_string(count) + string("\":\"");
		start = body.find(perm) + perm.length();
		end = body.find("\"",start);

		mode_t mod = stoi(body.substr(start,end-start),NULL);
		
		string p1 = (mod & S_IRUSR) ? "r" : "-";
		string p2 = (mod & S_IWUSR) ? "w" : "-";
		string p3 = (mod & S_IXUSR) ? "x" : "-";
		string p4 = (mod & S_IRGRP) ? "r" : "-";
		string p5 = (mod & S_IWGRP) ? "w" : "-";
		string p6 = (mod & S_IXGRP) ? "x" : "-";
		string p7 = (mod & S_IROTH) ? "r" : "-";
		string p8 = (mod & S_IWOTH) ? "w" : "-";
		string p9 = (mod & S_IXOTH) ? "x" : "-";

		string perms = d + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9; 


		// time
		string time = string("\"time") + to_string(count) + string("\":\"");
		start = body.find(time) + time.length();
		end = body.find("\"",start);

		
		time_t t = stoi(body.substr(start,end-start),NULL);
		struct tm *now = localtime(&t);
  		char buffer[TIME_BUF];

		strftime(buffer,TIME_BUF," %b %d %R",now);
	

		// links
		string links = string("\"links") + to_string(count) + string("\":\"");
		start = body.find(links) + links.length();
		end = body.find("\"",start);
		string l = body.substr(start,end-start);


		// username
		string user = string("\"user") + to_string(count) + string("\":\"");
		start = body.find(user) + user.length();
		end = body.find("\"",start);
		string usr = body.substr(start,end-start);

		struct passwd *username = getpwuid(stoi(usr,NULL));


		// groupname
		string group  = string("\"group") + to_string(count) + string("\":\"");
		start = body.find(group) + group.length();
		end = body.find("\"",start);
		string gr = body.substr(start,end-start);
		struct group *groupname = getgrgid(stoi(gr,NULL));

		
		output += perms + string(" ") + l + string(" ") + string(username->pw_name) + string(" ") + string(groupname->gr_name) + string(" ")  + sz + string("\t") + string(buffer) + string(" ") +  name + string("\n"); 

		body.erase(0,end);
	
		count++;
	}
	return output;

}

/**
 * Parses message received from a server. It uses error code and status that is 
 * in certain cases sent in a body of a response in JSON format. The errors and
 * possible output are redirected to standard output or in case of GET into the
 * given file.
 *
 * @param	answer		text of answer
 * @param	path		path to output file
 * @param	type		type of command
 * @param	filename	output filename
 *
 * @return	ERROR in case of error, else 0 	
 */
int parse_received(string head, string body, string path, command_t type, string filename){

	// Empty answer
	string status;
	string output;
	int sep_pos;
	int sep_end;

	// Correct protocol?
	int pos_b = 0;
	int pos_e = head.find(" ");
	string http = head.substr(pos_b,pos_e);
	if(http.compare(string("HTTP/1.1"))){
		cerr << "Unknown error.";
		return ERROR;
	}
	
	// Error code
	pos_b = pos_e + 1;
	pos_e = head.find(" ",pos_b);
	string error_code = head.substr(pos_b,pos_e-pos_b);

	switch(type){
		case GET:
			if(error_code.compare("200") == 0){
				if(path.back() != '/'){
					path += string("/");
				}
				ofstream out(path + filename,ios::binary);
				if(!out.is_open()){
					return ERROR;
				}
				out << body;
				out.close();
			}
			else{
				read_status(body,&status);
				write_err(status);
			}
			break;
		case LST:
			if(error_code.compare("200") == 0){
				while((size_t)(sep_pos = body.find(":\"")) != string::npos){
					sep_end = body.find("\"",sep_pos + 2);
					output += body.substr(sep_pos + 2,sep_end-sep_pos-2) + string("\t");
					body.erase(0,sep_end);
				}
				output.pop_back();
				if(output.length() != 0){
					output += string("\n");
				}
				cout << output;
			}
			else{
				read_status(body,&status);
				write_err(status);
				
			}
			break;
		case LSTMOR:
			if(error_code.compare("200") == 0){
				cout << parse_lst(body);
			}
			else{
				read_status(body,&status);
				write_err(status);
			}
			break;

		case DEL:
		case PUT:
		case CP:
		case MV:
		case MKD:	
		case RMD:
			if(error_code.compare("200") != 0){
				read_status(body,&status);
				write_err(status);
			}
			break;
		default:
			return ERROR;
	}
	return 0;

}

/**
 * Main function
 */
int main(int argc, char* argv[]){
	int socket;	
	string path_cl;		// Path to the file at the client side
	string path_svr;	// Path at server side (URI)
	command_t command = NONE;	// Individual commands
	int port = DEFAULT_PORT;	// Number of port
	string host = "localhost";	// Name of the host
	string user_path;	// Path after the hostname
	string request;		// HTTP request
	string file;		// String where the file is stored
	string filename;	// Name of a file
	int file_name_start;	// For indexing

	// Parses parameters
	if(argc == 3){
		if(strcmp(argv[1],"lst") == 0){
			path_svr = string(argv[2]);
			command = LST;
		}
		else if(strcmp(argv[1],"lst-more") == 0){
			path_svr = string(argv[2]);
			command = LSTMOR;
			file = string("{ \"type\":\"more\" }");
		}
		else if(strcmp(argv[1],"mkd") == 0){
			path_svr = string(argv[2]);
			command = MKD;
		}
		else if(strcmp(argv[1],"get") == 0){
			path_svr = string(argv[2]);
			path_cl = get_cwd();	// uses CWD
			file_name_start = path_svr.rfind(string("/")) + 1;
			filename = path_svr.substr(file_name_start);
			command = GET;
		}
		else if(strcmp(argv[1],"del") == 0){
			path_svr = string(argv[2]);
			command = DEL;
		}
		else if(strcmp(argv[1],"rmd") == 0){
			path_svr = string(argv[2]);
			command = RMD;
		}
		else{
			cerr << "Error: Invalid combination of parameters." << endl;
			return 1;
		}

	}
	else if(argc == 4){
		if(strcmp(argv[1],"put") == 0){
			path_svr = string(argv[2]);
			path_cl = string(argv[3]);

			// Searches for file to send
			struct stat file_stat;
			
			if (!(stat(path_cl.c_str(), &file_stat) == 0 && S_ISREG(file_stat.st_mode))){
				cerr << "Error: Invalid file." << endl;
				return 1;
			}
			if(get_file(path_cl,&file) == ERROR){
				cerr << "Error: Invalid file." << endl;
				return 1;
			}
			command = PUT;
		}
		else if(strcmp(argv[1],"get") == 0){
			path_svr = string(argv[2]);
			path_cl = string(argv[3]);

			// Is output direcotry valid?
			struct stat dir_stat;
			if (!(stat(path_cl.c_str(), &dir_stat) == 0 && S_ISDIR(dir_stat.st_mode))){
				cerr << "Error: Invalid directory." << endl;
				return 1;
			}
			if(path_cl.back() != '/'){
				path_cl += string("/");
			}
			file_name_start = path_svr.rfind(string("/")) + 1;
			filename = path_svr.substr(file_name_start);
			command = GET;
		}
		else if(strcmp(argv[1],"mv") == 0){
			path_svr = string(argv[2]);
			path_cl = string(argv[3]);
			file = string("{ \"type\":\"mv\", \"path\":\"") + path_cl + string("\" }");
			command = MV;
		}
		else if(strcmp(argv[1],"cp") == 0){
			path_svr = string(argv[2]);
			path_cl = string(argv[3]);
			file = string("{ \"type\":\"cp\", \"path\":\"") + path_cl + string("\" }");
			command = CP;
		}

		else{
			cerr << "Error: Invalid combination of parameters." << endl;
			return 1;
		}
	}
	else{
		cerr << "Error: Invalid combination of parameters." << endl;
		return 1;
	}
	
	if(parse_remote_path(path_svr,&port,&host,&user_path) == ERROR){
		return 1;
	}

	
	if(establish_connection(host,port,&socket) == ERROR){
		cerr << "Error: Connection was not established." << endl;
		return 1;
	}

	create_HTTP_header(&request,user_path,host,command,file.size());
	request += file;

	if(send(socket, request.c_str(), request.size(), 0) < 0){
		cerr << "Error: Request was not sent." << endl;
		return 1;
	}

	char buffer[BUF_SIZE];
	int n = 0;
        
	memset(buffer,0,BUF_SIZE);      
	string answer = string();

	// Receives a file
	string answer_body;
	string answer_head;
	int count = 0;
	int sent = 0;
	while ((n = recv(socket, buffer, BUF_SIZE, 0)) > 0){
		answer.append(buffer,n);
		if(count == 0){
			int pos = answer.find("\r\n\r\n");
			answer_head = answer.substr(0,pos);
			answer_body = answer.substr(pos+4);
			int pos_s;
			if((size_t)(pos_s = answer_head.find("Content-Length: "))!= string::npos){
				int pos_f = answer_head.find("\r\n",pos_s);
				string sent_size = answer_head.substr(pos_s + 16,pos_f-pos_s - 16);
				sent = stoi(sent_size,NULL);
				count += answer_body.size();
				if(answer_body.size() >= (unsigned)sent){
					break;
				}
			}
			else{
				break;
			}
		}
		else{
			answer_body.append(buffer,n);
			if(answer_body.size() >= (unsigned)sent){
				break;
			}
			bzero(buffer,BUF_SIZE);
		}
	}

	if(parse_received(answer_head,answer_body, path_cl, command, filename) == ERROR){
		cerr << "Error: Unrecognised answer." << endl;
	}
	close(socket);	

	return 0;
}

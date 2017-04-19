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
int establish_connection(int port, int *soc, struct sockaddr_in *server){
	if((*soc = socket(PF_INET, SOCK_STREAM, 0)) < 0){
		return ERROR;
	}

	server->sin_family = PF_INET;
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


	struct stat dir_stat;

	// Path must end with '/'
	if(path.back() != '/'){
		path += string("/");
	}
	
	char buffer[BUF_SIZE];
	int n = 0;

	// Receives request
	memset(buffer,0,BUF_SIZE);	
	string request = string();
	string body = string();
	string head = string();
	string aux;
	int count = 0;
	int sent = 0;
	while ((n = recv(acc_soc, buffer, BUF_SIZE, 0)) > 0){
		request.append(buffer,n);
		if(count == 0){
			int pos = request.find("\r\n\r\n");
			head = request.substr(0,pos);
			body = request.substr(pos+4);
			int pos_s;
			if((size_t)(pos_s = head.find("Content-Length: "))!= string::npos){
				int pos_f = head.find("\r\n",pos_s);
				string sent_size = head.substr(pos_s + 16,pos_f-pos_s - 16);
				sent = stoi(sent_size,NULL);
				count += body.size();
				if(body.size() >= (unsigned)sent){
					break;
				}
			}
			else{
				break;
			}
		}
		else{
			body.append(buffer,n);
			
		}
		if(body.size() >= (unsigned)sent){
			break;
		}
		bzero(buffer,BUF_SIZE);
	}

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

	//-------------user-not-fount------------//	
	if(!correct_user(path,username)){
		answer_return = string("404 NOT FOUND");
		answer_body = string("{ \"status\":\"user_not_found\" }");
		answer_type = string("application/json");
	}

	//-------------put------------//	
	else if(!command.compare("PUT") && !type.compare("file") && !sent_type.compare("application/octet-stream")){
		file_path = path + user_path;
		file_dir = file_path.substr(0,file_path.rfind("/"));
		answer_type = string("application/json");
		/*if(!(stat(file_dir.c_str(), &dir_stat) == 0 && S_ISDIR(dir_stat.st_mode))){
			answer_return = string("404 NOT FOUND");
			answer_body = string("{ \"status\":\"unavailable_dir\" }");
		}*/
		if(stat(file_path.c_str(), &dir_stat) == 0 /* && S_ISREG(dir_stat.st_mode)*/){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"already_exists\" }");
		}
		else{
			ofstream out(file_path,ios::binary);
			if(!out.is_open()){
				answer_return = string("404 NOT FOUND");
				answer_body = string("{ \"status\":\"unknown_error\" }");
			}
			else{
				out << body;
				out.close();
				answer_return = string("200 OK");
				answer_body = string("{ \"status\":\"ok\" }");
			}
		}
	}

	//-------------mv-cp-----------//	
	else if(!command.compare("PUT") && !type.compare("file") &&
	!sent_type.compare("application/json")){
		
		answer_type = string("application/json");
		file_path = path + user_path;
		string filename = file_path.substr(file_path.rfind("/") + 1);
		int start = body.find("\"type\":\"") + 8;
		int end = body.find("\"",start);

	       	string cp_mv = body.substr(start,end-start);
		
		start = body.find("\"path\":\"") + 8;
		end = body.find("\"",start);
		string output_path = path + username + string("/");
		output_path += body.substr(start,end-start);
		
		string output_dir = output_path.substr(0,output_path.rfind("/"));

		if (stat(file_path.c_str(), &dir_stat)){
			answer_return = string("404 NOT FOUND");
			answer_body = string("{ \"status\":\"file_not_found\" }");
		}
		else if(!S_ISREG(dir_stat.st_mode)){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"not_file\" }");
		}
		else if(!(stat(output_dir.c_str(), &dir_stat) == 0 && S_ISDIR(dir_stat.st_mode))){
			answer_return = string("404 NOT FOUND");
			answer_body = string("{ \"status\":\"unavailable_dir\" }");
		}
		else if(stat(output_path.c_str(), &dir_stat) == 0){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"already_exists\" }");
		}
		else{
			ifstream ins(file_path,ios::binary);
			ofstream out(output_path,ios::binary);
			if(!out.is_open() || !ins.is_open()){
				answer_return = string("404 NOT FOUND");
				answer_body = string("{ \"status\":\"unknown_error\" }");
			}
			else{
				out << ins.rdbuf();
				out.close();
				ins.close();
				answer_return = string("200 OK");
				answer_body = string("{ \"status\":\"ok\" }");
				
				// mv removes file
				if(!cp_mv.compare("mv")){
					remove(file_path.c_str());
				}
			}
		}
	}
	//-------------get------------//	
	else if(!command.compare("GET") && !type.compare("file")){
		user_path.pop_back();
		file_path = path + user_path;
		answer_type = string("application/json");
		
		if (stat(file_path.c_str(), &dir_stat)){
			answer_return = string("404 NOT FOUND");
			answer_body = string("{ \"status\":\"file_not_found\" }");
		}
		else if(!S_ISREG(dir_stat.st_mode)){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"not_file\" }");
		}
		else{
			answer_type = string("application/octet-stream");
			ifstream fin(file_path, ios::binary);
			if(!fin.is_open()){
				answer_return = string("404 NOT FOUND");
				answer_body = string("{ \"status\":\"unknown_error\" }");
			}
			else{
				ostringstream ostrm;
				ostrm << fin.rdbuf();
				answer_body = ostrm.str();
				fin.close();
				answer_return = string("200 OK");
				size = to_string(answer_body.size());
			}
		}	
	}

	//-------------del------------//	
	else if(!command.compare("DELETE") && !type.compare("file")){
		user_path.pop_back();
		file_path = path + user_path;
		answer_type = string("application/json");

		if (stat(file_path.c_str(), &dir_stat)){
			answer_return = string("404 NOT FOUND");
			answer_body = string("{ \"status\":\"file_not_found\" }");
		}
		else if(!S_ISREG(dir_stat.st_mode)){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"not_file\" }");
		}
		else{
			if(remove(file_path.c_str()) != 0){
				answer_return = string("400 BAD REQUEST");
				answer_body = string("{ \"status\":\"missing_permissions\" }");
			}
			else{
				answer_return = string("200 OK");
				answer_body = string("{ \"status\":\"ok\" }");
			}
		}
	}

	//-------------lst-lst-more-----------//	
	else if(!command.compare("GET") && !type.compare("folder")){
		string type("normal");
		string found_file;
		if(body.find("\"type\":\"") != string::npos){
			int start = body.find("\"type\":\"") + 8;
			int end = body.rfind("\"");	
	       		type = body.substr(start,end-start);
		}

		answer_type = string("application/json");
		file_path = path + user_path;
		file_path.pop_back();
		if (stat(file_path.c_str(), &dir_stat)){
			answer_return = string("404 NOT FOUND");
			answer_body = string("{ \"status\":\"dir_not_found\" }");
		}
		else if(!S_ISDIR(dir_stat.st_mode)){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"not_dir\" }");
		}
		else{
			file_path += string("/");
			struct dirent *dir_entity;
			DIR *dir_obj;
			dir_obj = opendir(file_path.c_str());
			if (dir_obj == NULL){
				cerr << "Invalid path." << endl;
				answer_return = string("400 BAD REQUEST");
				answer_body = string("{ \"status\":\"missing_permission\" }");
			}
			else{
				answer_body = string("{");
				int count = 0;

				// Iterates directory
				while ((dir_entity = readdir(dir_obj)) != NULL) {
					if(strcmp(dir_entity->d_name,".") && 
					strcmp(dir_entity->d_name,"..")){
						found_file = file_path;
						found_file += string(dir_entity->d_name);
						answer_body += make_json_element(string("element") + to_string(count),string(dir_entity->d_name));
						// lst-more
						if(!type.compare("more")){
							stat(found_file.c_str(), &dir_stat);
							string key = string("size")+to_string(count);
							string value = to_string(dir_stat.st_size);
							answer_body += make_json_element(key,value);
							key = string("type")+to_string(count);
							if(S_ISDIR(dir_stat.st_mode)){
								value = string("dir");
							}
							else if(S_ISREG(dir_stat.st_mode)){
								value = string("file");
							}
							else{
								value = string("undef");
							}
							answer_body += make_json_element(key,value);
							key = string("perm")+to_string(count);
							value = to_string(dir_stat.st_mode);
							answer_body += make_json_element(key,value);
							key = string("time")+to_string(count);
							value = to_string(dir_stat.st_ctime);
							answer_body += make_json_element(key,value);
							key = string("links")+to_string(count);
							value = to_string(dir_stat.st_nlink);
							answer_body += make_json_element(key,value);
							key = string("user")+to_string(count);
							value = to_string(dir_stat.st_uid);
							answer_body += make_json_element(key,value);
							key = string("group")+to_string(count);
							value = to_string(dir_stat.st_gid);
							answer_body += make_json_element(key,value);



						}
						count++;
					}
				}
				if(count == 0){
					answer_body += string(" \"element\":\"\" ");
				}
				answer_body.pop_back();
				answer_body += string(" }");
				closedir (dir_obj);
				answer_return = string("200 OK");
			}
		}
	}

	//-------------mkd------------//	
	else if(!command.compare("PUT") && !type.compare("folder")){
		answer_type = string("application/json");
		file_path = path + user_path;
		
		int mkdir_err = mkdir(file_path.c_str(),S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		if(mkdir_err == -1){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"already_exists\" }");
		}
		else{
			answer_return = string("200 OK");
			answer_body = string("{ \"status\":\"ok\" }");
		}
	}
	//-------------rmd------------//	
	else if(!command.compare("DELETE") && !type.compare("folder")){
		answer_type = string("application/json");
		file_path = path + user_path;
		file_path.pop_back();
		user_path.pop_back();
		if(user_path.compare(username) == 0){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"user\" }");
		}
		else if (stat(file_path.c_str(), &dir_stat)){
			answer_return = string("404 NOT FOUND");
			answer_body = string("{ \"status\":\"dir_not_found\" }");
		}
		else if(!S_ISDIR(dir_stat.st_mode)){
			answer_return = string("400 BAD REQUEST");
			answer_body = string("{ \"status\":\"not_dir\" }");
		}
		else{
			int rmdir_err = rmdir(file_path.c_str());
			if(rmdir_err == -1){
				answer_return = string("400 BAD REQUEST");
				answer_body = string("{ \"status\":\"not_empty\" }");
			}
			else{
				answer_return = string("200 OK");
				answer_body = string("{ \"status\":\"ok\" }");
			}
		}	
	}
	
	// timestamp
	time_t t = time(0);
        struct tm *now = localtime(&t);
        char time_buffer[TIME_BUF];

        strftime(time_buffer,TIME_BUF,"%a, %d %b %G %T %Z",now);

	// generates header
	string answer_head = string("HTTP/1.1 ") + answer_return + string("\r\n");	
	answer_head += string("Date: ") + string(time_buffer) + string("\r\n");
	answer_head += string("Content-Type: ") + answer_type + ("\r\n");
	answer_head += string("Content-Length: ") + to_string(answer_body.size()) + string("\r\n");
	answer_head += string("Content-Encoding: identity");
	answer_head += string("\r\n\r\n");
	answer += answer_head;
	answer += answer_body;
	if(send(acc_soc,answer.c_str(),answer.size(),0) < 0){
		cerr << "Answer was not sent.";
		return ERROR;
	}
	return 0;

}

/**
 * Returns current working directory.
 *
 * @return	current working directory or NULL in case of error
 */
string get_cwd(){
	char path[MAX_PATH];

	if (getcwd(path, MAX_PATH) != 0) 
		return string (path);
	return NULL;
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
	struct sockaddr_in server;
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

/*

 Simple echo server that converts upper/lower case letters
 - with a dynamic port number
 - iterative server (accepts connection one by one)
 - CTRL-C stops running the server

 Usage: echo-server1 <port number>

*/
 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<err.h>
#include<ctype.h>

#define BUFFER 1024         // buffer for incoming messages
#define QUEUE 1             // queue length for  waiting connections
 
int main(int argc , char *argv[]){ // expect one parameter: port number
  int fd;
  int newsock;
  int len, msg_size, i;
  struct sockaddr_in server;   // the server configuration (socket info)
  struct sockaddr_in from;     // configuration of an incoming client (socket info)
  char buffer[BUFFER];
     
  
  if (argc != 2)               // read the first parameter: a port numberq
    errx(1,"Usage %s <port number>",argv[0]);

  if ((fd = socket(AF_INET , SOCK_STREAM , 0)) == -1)   //create a server socket
    err(1,"socket(): could not create the socket");
  
  printf("The socket successfully created by socket()\n");

  server.sin_family = AF_INET;             // initialize server's sockaddr_in structure
  server.sin_addr.s_addr = INADDR_ANY;     // wait on every network interface, see <netinet/in.h>
  server.sin_port = htons(atoi(argv[1]));  // set the port where server is waiting
     

  if( bind(fd,(struct sockaddr *)&server , sizeof(server)) < 0)  //bind the socket to the port
    err(1,"bind() failed");

  printf("socket succesfully bounded to the port using bind()\n");
     
  if (listen(fd,QUEUE) != 0)   //set a queue for incoming connections
    err(1,"listen() failed");
     
  printf("Waiting for incoming connections on port %d (%d)...\n", atoi(argv[1]),server.sin_port);
     
  // accept connection from an incoming client
  // parameter "from" gets information about the client
  // - if we don't need it, put NULL instead; e.g., newsock=accept(fd,NULL,NULL);
  // newsock is a file descriptor to a new socket where incoming connection is processed

  while(1){  // wait for incoming connections (iterative server)

    if ((newsock = accept(fd, (struct sockaddr *)&from, (socklen_t*)&len)) == -1)
      err(1,"accept failed");
    
    printf("A new connection accepted from %s, port %d (%d)\n",inet_ntoa(from.sin_addr),ntohs(from.sin_port), from.sin_port);
    
    // process incoming messages from the client using "newsock" socket
    // until the client stops sending data (CRLF)

    while((msg_size = read(newsock, buffer, BUFFER)) > 0){ // read the message
      printf("buffer = \"%.*s\"\n",msg_size,buffer);
      
      for (i = 0; i < msg_size; i++)          // convert data to upper/lower case
	if (islower(buffer[i]))
	  buffer[i] = toupper(buffer[i]);
	else if (isupper(buffer[i]))
	  buffer[i] = tolower(buffer[i]);
      
      i = write(newsock,buffer,msg_size);    // send a converted message to the client
      if (i == -1)                           // check if data was successfully sent out
	err(1,"write() failed.");
      else if (i != msg_size)
	err(1,"write(): buffer written partially");
    }

    // no other data from the client -> close the client and wait for another

    close(newsock);                          // close the new socket
    printf("closing newsock\n");
  }

  // close the server 
  close(fd);                               // close an original server socket
  printf("closing an original socket\n");
  return 0;
}

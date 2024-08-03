#include "server.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

//Initializes the Server struct and binds the server socket
struct Server server_constructor(int domain, int type, int protocol, u_long interface, int port, int backlog, void(*launch)(struct Server* server)){
	struct Server server;

	server.domain = domain;
	server.type = type;
	server.protocol = protocol;

	server.address.sin_addr.s_addr = htonl(interface);
	server.address.sin_family = domain;
	server.address.sin_port = htons(port);

	server.socket = socket(server.domain, server.type, server.protocol); 

	if(server.socket < 0){
		perror("Failed to create socket\n");
		exit(1);
	}

	if(bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address)) < 0){
		perror("Failed to bind socket\n");
		exit(1);
	}

	if(listen(server.socket, server.backlog) < 0){
		perror("Failed to lister\n");
		exit(1);
	}
	
	//A launch function provided by the server_constructor()
	server.launch = launch;

	return server;
}	

//Function to handle individual connections
void* handler(void* ptr){

		char reqbuff[1024];
		char *resp200 = "HTTP/1.1 200 OK\r\n\r\nHello";
		int clientfd = *(int *)ptr;

		read(clientfd, reqbuff, sizeof(reqbuff));
		printf("%s\n", reqbuff);	

		write(clientfd, resp200, strlen(resp200));

		close(clientfd);
		
		printf("\n========Client Disconnected========\n");

		return NULL;
}

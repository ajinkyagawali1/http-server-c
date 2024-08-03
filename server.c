#include "server.h"
#include"httprequest.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
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

	//settting socket option to ignore the TIME_WAIT for port/address
	setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, 0, 0);

	if(listen(server.socket, server.backlog) < 0){
		perror("Failed to lister\n");
		exit(1);
	}
	
	//A launch function provided by the server_constructor()
	server.launch = launch;

	return server;
}	

//function to verify requested path
bool is_path(char* path){
	if(access(path, F_OK) == 0){ 
		perror("Path exists\n");
		return true;
	}
	else{
		perror("Path does not exist\n");
		return false;
	}	
}

//Function to handle individual connections
void* handler(void* ptr){

		char reqbuff[1024];
		char *resp404 = "HTTP/1.1 404 File not found\r\n\r\nFile not Found";
		int clientfd = *(int *)ptr;

		read(clientfd, reqbuff, sizeof(reqbuff));
		printf("Request: %s\n\n", reqbuff);	

		struct httprequest request;

		hreqparser(reqbuff, &request);
		printf("Path: %s\n", request.path);
		printf("Hostname: %s\n", request.host);
		printf("Accept-encoding: %s\n", request.acceptencoding);
		printf("Accept: %s\n", request.acceptformat);
		printf("User-agent: %s\n", request.user_agent);
	
		char tmp[512] = "/home/ajinkya/Projects/http-server/static";
		char* actual_path = strcat(tmp, request.path);
		printf("ActualPath: %s\n", actual_path);

		if(is_path(actual_path)){
			FILE *file;
			file = fopen(actual_path, "r");
		
			if(file == NULL){
				perror("file");
				close(clientfd);
				printf("\n========Client Disconnected========\n");
				return NULL;
			}

			fseek (file, 0, SEEK_END);
			long length = ftell (file);
			fseek (file, 0, SEEK_SET);
			char *buffer = malloc(length);		
			
			fread(buffer, 1, length, file);
			printf("Sendbuff: %s\n", buffer);				
			char response[3000];

			sprintf(response, "HTTP/1.1 200 OK/r/nContent-Type: text/html; charset=utf8\r\nContent-Length: %d\n\n\%s", length, buffer);
			free(buffer);
			write(clientfd, response, strlen(response));
		}else{
		write(clientfd, resp404, strlen(resp404));
		}
		close(clientfd);
		
		printf("\n========Client Disconnected========\n");

		return NULL;
}

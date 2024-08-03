#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<limits.h>
#include<string.h>
#include"server.h"

void launch(struct Server *server){

	int addrlen = sizeof(server->address);

	char reqbuff[1024];

	while(1){

		printf("\n=======Waiting for Connections========\n\n");

		int clientfd = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&addrlen);

		if(clientfd < 0){
			perror("Failed to accept Connection\n");
			exit(1);
		}	      

		printf("\n=========Client Accepted==========\n");

		int *pclientfd = malloc(sizeof(clientfd));
		*pclientfd = clientfd;		
		pthread_t thread;
		pthread_create(&thread, NULL, handler, pclientfd);
	}
}

int main(int argc, char* argv[]){

	if(argc != 2){
		fprintf(stderr, "Usage: %s [port]]\n", argv[0]);
		exit(EXIT_FAILURE);	
	}
	
	errno = 0;
	char *nptr = argv[1], *endptr;
	long conv = strtol(nptr, &endptr, 10);

	if (errno != 0 || *endptr  != '\0' || conv > INT_MAX || conv < INT_MIN) {
		fprintf(stderr, "The port specified is not valid\n");
		exit(EXIT_FAILURE);
	}

	int port = conv;

	struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, port, 10, launch);

	server.launch(&server);
}

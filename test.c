#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<stdio.h>
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

		pthread_t thread;
		pthread_create(&thread, NULL, handler, &clientfd);
		//pthread_join(thread, NULL);
	}
}

int main(){
	struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 10, launch);

	server.launch(&server);

	return 0;
}

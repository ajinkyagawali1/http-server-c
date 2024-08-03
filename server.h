#include<netinet/in.h>

struct Server{

	int domain;
	int type;
	int protocol;
	u_long interface;
	int port;
	int backlog;
	int socket;

	struct sockaddr_in address;

	void (*launch)(struct Server*);
};

struct Server server_constructor(int domain, int type, int protocol, u_long interface,  int port, int backlog, void (*launch)(struct Server*));

void * handler(void* ptr);

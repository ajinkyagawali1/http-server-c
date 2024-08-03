#include "httprequest.h"
#include<string.h> 
#include<stdio.h>

// Initializing request struct
//struct httprequest request;

// Array of header fields to match against
char * hfields[5]={"Host", "User-Agent", "Accept", "Accept-Language", "Accept-Encoding"};

// Function used by parser to compare tokens with header field values
int hcompare(char*, char**);

//Main Parser function
void hreqparser(char* req, struct httprequest* request){

	for(int i=0; i<strlen(req) -2; i++){
		if(req[i] == '\n' && req[i+1] == '\n'){
			req[i+1] = '|';
		}
	}	

	char* firstline = strtok(req, "\n");
	char* headers = strtok(NULL, "|");
	char* body = strtok(NULL, "|");
	
	request->content = body;

	request->method = strtok(firstline, " ");

	request->path = strtok(NULL, " ");

	request->hversion = strtok(NULL, "\r\n");	

	char* token = strtok(headers, "\r\n");

	while(token){

		int i = hcompare(token, hfields);
		printf("index: %d\n", i);

		switch(i){

			case 0:
				request->host = token+6;
				break;
			case 1:
				request->user_agent = token+12;
				break;
			case 2:
				request->acceptformat = token+8;
				break;
			case 3:
				request->acceptlanguage = token+17;
				break;
			case 4:
				request->acceptencoding = token+17;	
				break;
			case -1:
				break;	
		}
		token = strtok(NULL, "\r\n");
	}
}

//Compare function definition
int hcompare(char* token, char** hfields){
	
	int key = -1;

	for(int i=0; i<5; i++){
		if(strstr(token, hfields[i]) != NULL){
			key = i;
		}
	}
	return key; 
}

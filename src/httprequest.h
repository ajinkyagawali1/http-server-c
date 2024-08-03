// For storing all components of http request 
struct httprequest{
	char* method;
	char* path;
	char* hversion;
	char* host;
	char* user_agent;
	char* acceptformat;
	char* acceptlanguage;
	char* acceptencoding;
	char* content;
};

// For parsing http request buffer into its components
void hreqparser(char* req, struct httprequest*);


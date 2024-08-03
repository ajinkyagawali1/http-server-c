main: test.o server.o
	gcc -o main test.o server.o httprequest.o

test.o: test.c
	gcc -o test.o -c test.c 

server.o: server.c
	gcc -o server.o -c server.c

httprequest.o: httprequest.c
	gcc -o httprequest.o -c httprequest.c

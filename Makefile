
CC = gcc
o = -o
lp = -lpthread
flag = -Wall -g -c


# make for the code.
all: server client

Node.o: Node.c
		$(CC) Node.c $(flag) Node.c

#test.o: Queue.o test.cpp
#		$(CC) test.cpp $(lp) $(o) test

server: Queue.o server.cpp
		$(CC) server.cpp $(lp)  $(o) server

client: client.cpp
		$(CC) client.cpp $(lp) $(o) client

Queue.o: Node.o Queue.c
		$(CC) Queue.c $(lp) $(flag) Queue.c

# make clean
clean:
	rm -f *.o test server client
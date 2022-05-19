
CC = gcc
o = -o
lp = -lpthread
flag = -Wall -g -c


# make for the code.
all: test.o server.o client.o

Node.o: Node.c
		$(CC) Node.c $(flag) Node.c

test.o: Queue.o test.cpp
		$(CC) test.cpp $(lp) $(o) test

Queue.o: Node.o Queue.c
		$(CC) Queue.c $(lp) $(flag) Queue.c

server.o: Queue.o server.cpp
		$(CC) server.cpp $(lp) $(o) server

client.o: client.cpp
		$(CC) client.cpp $(lp) $(o) client




# make clean
clean:
	rm -f *.o test server client
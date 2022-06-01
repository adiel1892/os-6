
CC = gcc
gg = g++
o = -o
lp = -lpthread
flag = -Wall -g -c


# make for the code.
all: server client main1 guard singelton

main1: Queue.o main1.c
		$(gg)  main1.c $(lp)  $(o) main1

Node.o: Node.c
		$(CC) Node.c $(flag) Node.c

#test.o: Queue.o test.cpp
#		$(CC) test.cpp $(lp) $(o) test
guard:
		$(gg) guard.cpp -o guard $(lp)

singelton:
		$(gg) singelton.cpp -o singelton $(lp)

server: selectserver.c
		$(CC) selectserver.c $(lp)  $(o) server

client: selectclient.c
		$(CC) selectclient.c $(lp) $(o) client

Queue.o: Node.o Queue.c
		$(CC) Queue.c $(lp) $(flag) Queue.c

# make clean
clean:
	rm -f *.o test server client main1 guard singelton
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>
#include "Queue.c"
#include <string.h>
#include <stdlib.h>
#define MAX_LENGTH 1024
#define PORT "3492"  // the port users will be connecting to
#define BACKLOG 10   // how many pending connections queue will hold
 Queue* firstQueue = createQ();
Queue* secondQueue = createQ();
Queue* thirdQueue = createQ();
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;
    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}

void destroyAO(pthread_t thread,Queue * q){
    destroyQ(q);
    pthread_cancel(thread);
}

void * firstAct(void * word,int len) {
    for (int i = 0; i < len; ++i) {
        if (((char*)word)[i] <= 'Z') {
            ((char*)word)[i] = (((char*)word)[i] + 1 - 65) % 26 + 65;//make every char shift by one but keep them in
            //in range of A-Z.
        } else {
            ((char*)word)[i] = (((char*)word)[i] + 1 - 97) % 26 + 97;
        }
    }
    return word;
}
void* (*functionPtrfirst)(void*,int)= &firstAct;
void *secondAct(void *word,int len){
    for (int i = 0; i < len; ++i) {
        if(((char*)word)[i]<='Z') {
            ((char*)word)[i] = ((char*)word)[i]+32;//make every big letter to small by asci value
        }else{
            ((char*)word)[i] = ((char*)word)[i]-32;//make every small letter to bih by asci value
        }
    }
    return word;
}
void newAO_3(int sock,int len){
    while(thirdQueue->size!=0){
        void * word_before = deQ(thirdQueue);
        send(sock, word_before, len-1, 0);
    }
}

void newAO_2(int sock,int len){
    while(secondQueue->size!=0){
        void * word_before = deQ(secondQueue);
        void * word_after = secondAct(word_before,len);
        enQ(thirdQueue,word_after,len);
        newAO_3(sock,len);
    }
}
void newAO_1(int sock,int len,Queue * q,void*(*before)(void *,int),void(*second_ActiveObj)(int,int)){
    while (q->size!=0){
        void* word_before = deQ(q);
        void* after_word = before(word_before,len);
        enQ(secondQueue,after_word,len);
        //maybe should add those thrid gotta check it
        //pthread_t thread_id;
        //pthread_create(&thread_id, NULL, second_ActiveObj, (void*)new_fd);
        second_ActiveObj(sock,len);

    }
}

void *handelClients(void * newfd) {
    long int newfd1 = (long int) newfd;
    while (1) {
        char user_input[MAX_LENGTH];
        char user_push[5];
        char dq_eq[8];
        char rest[MAX_LENGTH];
        char user_pop_top[4];
        memset(user_input, 0, MAX_LENGTH);
        memset(user_pop_top, 0, 4);
        memset(rest, 0, MAX_LENGTH);
        memset(dq_eq,0,8);
        recv(newfd1, user_input, MAX_LENGTH, 0);
        strncpy(user_pop_top, user_input, 3);
        strncpy(user_push, user_input, 4);
        strncpy(dq_eq,user_input,7);
        pthread_mutex_lock(&lock);
        enQ(firstQueue , (void*)user_input , strlen(user_input));
        newAO_1(newfd1,strlen(user_input),firstQueue,*firstAct,*newAO_2);
        pthread_mutex_unlock(&lock);
    }
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];
    int rv;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");
    int couneter = 0;

    while(1) {  // main accept() loop
        couneter++;
        sin_size = sizeof their_addr;

        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handelClients, (void*)new_fd);
        if(couneter>10){
            break;

        }

    }

    return 0;
}
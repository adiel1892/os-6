#include <pthread.h>
#include <stdlib.h>
typedef struct reactor{
    pthread_t thread_num;
    int fd;
    void (* handle_func)(void*);
}reactor;

void * newReactor(){
    reactor * rec = (reactor *)malloc(sizeof (reactor));
    return rec;
}
void InstallHandler(reactor * rec,void (*handle_func)(void*),int fd){
    rec->fd = fd;
    rec->handle_func = handle_func;
    pthread_create(&rec->thread_num,NULL,rec->handle_func,(void*)rec->fd);
}
void RemoveHandler(reactor * rec){
    rec->fd =-1;

}
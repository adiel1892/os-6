#include "Node.c"
#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
typedef struct Queue{
    Node *head;
    Node *tail;
    int size;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
}Queue;

Queue* createQ(){
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    pthread_mutex_t *lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock , NULL);
    pthread_cond_t *cond1 = (pthread_cond_t*) malloc(sizeof(pthread_cond_t));
    pthread_cond_init(cond1 , NULL);
    q->lock = lock;
    q->cond = cond1;
    return q;
}


void destroyQ(Queue *q){
    pthread_mutex_destroy(q->lock);
    free(q->lock);
    while(q->head != NULL){
        Node* tmp = q->head->next;
        q->head = q->head->next;
        free(tmp);
    }
}

void enQ(Queue *q , void* data, int data_size){
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = malloc(1024);
    for(int i = 0; i < data_size; i++){
        ((char*)newNode->data)[i] = ((char*)data)[i];
    }
    // newNode->data = data;
    newNode->next = NULL;
    pthread_mutex_lock(q->lock);
    if (!q->head) {
        q->head = newNode;
        q->tail = newNode;
        pthread_cond_signal(q->cond);

    } else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->size++;
    pthread_mutex_unlock(q->lock);
    pthread_cond_signal(q->cond);
}
void* deQ(Queue *q){
    pthread_mutex_lock(q->lock);
    void* tmp;
    if (q->head  == NULL) {
        pthread_cond_wait(q->cond, q->lock);
        tmp = NULL;
    }
    else{
        Node* node = q->head;
        q->head = node->next;
        void* returnValP = node->data;
        free(node);
        tmp = returnValP;
        q->size--;
    }
    pthread_mutex_unlock(q->lock);
    return tmp;

}
void* top(Queue *q){
    if(q->size > 0){
        return q->head->data;
    }


}

// *void before(){

// }

// *void after(){

// }

// void newAO(Queue q , void* before(void*) , void* after(void*)){
//     while(q.size > 0){
//         // q.pop -> before
//         // pelet from before -> after


//     }
// }



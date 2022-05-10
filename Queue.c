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
}Queue;



Queue* createQ(){
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    pthread_mutex_t *lock = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(lock , NULL);
    q->lock = lock;
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

void enQ(Queue *q , void* data){
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    pthread_mutex_lock(q->lock);
    if (!q->head) {
        q->head = newNode;
        q->tail = newNode;
    } else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->size++;
    pthread_mutex_unlock(q->lock);
}
void* deQ(Queue *q){
    pthread_mutex_lock(q->lock);
    void* tmp;
    if (q->head  == NULL) {
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

// int main(){

//     return 0;
// }



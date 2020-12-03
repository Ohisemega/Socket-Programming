#include "query.h"
#ifndef QUEUE_H
#define QUEUE_H
#include<semaphore.h>

typedef struct NodeStruct
{
   void* item;
   struct NodeStruct* next;
}Node;

//
typedef struct QueueStruct
{
    pthread_mutex_t lock;
    sem_t get;
    sem_t put;
    Node *head;
    Node *tail;
}Queue;

//

typedef struct Context
{
    Queue *toDo;
    Queue *done;
    pthread_t *threads;
    int workerNum;
}Context;

typedef struct Task
{
    char *url;
    Buffer *result;
}Task;


Queue *queueAlloc(int size);

void freeQueue(Queue *queue);

void placeQueue(Queue *queue, void* item);

void *fetchQueue(Queue* queue);

#endif
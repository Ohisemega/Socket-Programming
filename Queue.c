#include<pthread.h>
#include<semaphore.h>
#include<stdlib.h>
#include<stdio.h>
#include<errno.h>
#include<assert.h>
#include "Queue.h"

int checker1 (int func, char * sentence)
{
  if (func == -1)
  {
  puts(sentence);
  exit(EXIT_FAILURE);
  }
}

void error1(char* message)
{
    puts(message);
    exit(EXIT_FAILURE);
}
//
Queue *queueAlloc(int size)
{
    Queue* queue = malloc(sizeof(Queue));
    if (queue == NULL) error1("malloc: allocating queue failed");
     
     int errorCheck;
     if ((errorCheck = pthread_mutex_init(&queue->lock, NULL)) != 0) checker1(errorCheck, "pthread_mutex_init: failed");
     if (sem_init(&queue->get, 0, 0)==-1) error1("sem_init: failed");
     if (sem_init(&queue->put, 0, 0) == -1) error1("sem_init: failed");
     queue->head = NULL;
     queue->tail = NULL;
     return queue;
}

//
void freeQueue(Queue *queue)
{
    while(queue->head != NULL)
    {
        Node* temp = queue->head;
        queue->head = queue->head->next;
        free(temp);
    }
    int errorCheck;
    if((errorCheck = pthread_mutex_destroy(&queue->lock)) != 0) error1("pthread_mutex_destroy: failed");
    if((errorCheck = sem_destroy(&queue->get)) != -1) error1("sem_close : failed");
    if((errorCheck = sem_destroy(&queue->put)) != -1) error1("sem_close : failed");
    free(queue);
}

//
void placeQueue(Queue *queue, void* item)
{
   Node *node = malloc(sizeof(Node));
   if(node == NULL) error1("malloc: allocating node failed");
   node->item = item;
   node->next  = NULL;

   int errorChecker2;
   if((errorChecker2 = sem_wait(&queue->put)) == -1) error1("sem_wait: failed");
   if((errorChecker2 = pthread_mutex_lock(&queue->lock)) == -1) error1("sem_wait: failed");
   if(queue->head == NULL)//que is length 0
   {
      queue->head = node;
      queue->tail = node;
   }
   else
   {
     queue->tail->next = node;
     queue->tail = node;       
   }
    if ((errorChecker2 = pthread_mutex_unlock(&queue->lock)) != 0)  error1("pthread_mutex_unlock: failed");
    errorChecker2 = sem_post(&queue->get);
    checker1(errorChecker2, "sem_postfailed");
}

void *fetchQueue(Queue *queue)
{
   Node* node = NULL;
   int errorChecker3;
   errorChecker3 = sem_wait(&queue->get);
   checker1(errorChecker3, "sem_wait: failed");
   errorChecker3 = pthread_mutex_lock(&queue->lock);
   if(errorChecker3 != 0) error1("pthread_mutex_lock: failed");
   
   node = queue->head;
   queue->head = queue->head->next;
   if(queue->head ==NULL) queue->tail = NULL;

   errorChecker3 = pthread_mutex_unlock(&queue->lock);
   if(errorChecker3 != 0) error1("pthread_mutex_unock: failed");
   errorChecker3 = sem_post(&queue->put);
   checker1(errorChecker3, "sem_post: failed");

   void* item = node->item;
   free(node);

   //return item;
}


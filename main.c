// Client side C/C++ program to demonstrate Multi-threaded Socket programming 
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include <sys/types.h>
#include<sys/stat.h>
#include <string.h>
#include <pthread.h>
#include "Queue.h"
#define FileSize 256

int checkerMain (int func, char * sentence)
{
  if (func == -1)
  {
  puts(sentence);
  exit(EXIT_FAILURE);
  }
}

void errorMain(char* message)
{
    puts(message);
    exit(EXIT_FAILURE);
}

void createDirectory(const char *dir)
{
   struct stat st = {0};

   if(stat(&dir, &st))
   {
       int record = mkdir(dir, 0700);
       checkerMain(record, "mkdir: failed");
   }
}

void *workerThread(void *arg)
{
   Context *content = (Context *)arg;
   Task *task = (Task *)fetchQueue(content->toDo);

   while(task)
   {
       task->result = httpURL(task->url);
       placeQueue(content->done, task);
       task = (Task *)fetchQueue(content->toDo);
   }
   return NULL;
}

Context *spawnWorker(int workerNum) 
{
    Context *content = (Context*)malloc(sizeof(Context));
    content->toDo = queueAlloc(workerNum * 2);
    content->done = queueAlloc(workerNum * 2);
    content->workerNum = workerNum;
    content->threads = (pthread_t*)malloc(sizeof(pthread_t) * workerNum);
    int i = 0;

    for (i = 0; i < workerNum; ++i) {
        if (pthread_create(&content->threads[i], NULL, workerThread, content) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    return content;
}

void freeWorker(Context *content) 
{
    int workerNum = content->workerNum;
    int i = 0;

    for (i = 0; i < workerNum; ++i) {
        placeQueue(content->toDo, NULL);
    }

    for (i = 0; i < workerNum; ++i) {
        if (pthread_join(content->threads[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }

    freeQueue(content->toDo);
    freeQueue(content->done);

    free(content->threads);
    free(content);
}

Task *newTask(char *url) 
{
    Task *task = malloc(sizeof(Task));
    task->result = NULL;
    task->url = malloc(strlen(url) + 1);

    strcpy(task->url, url);

    return task;
}

void freeTask(Task *task) 
{    
    if (task->result) 
    {
        free(task->result->data);
        free(task->result);
    }

    free(task->url);
    free(task);
}

void taskWaiting(const char *downDirec, Context *context) {
    char filename[FileSize], fileURL[FileSize];
    Task *task = (Task*)fetchQueue(context->done);

    if (task->result) {

        strcpy(fileURL, task->url);

        size_t len = strlen(fileURL);
        for (int i = 0; i < len; ++i) 
        {
            if (fileURL[i] == '/') 
            {
                fileURL[i] = '-';
            }
        }

        snprintf(filename, FileSize, "%s/%s", downDirec, fileURL);
        char *data = httpGetRequest(task->result);
        if (data) {
            size_t length = task->result->length - (data - task->result->data);
			FILE *fp = fopen(filename, "w");
			if (fp == NULL) {
				fprintf(stderr, "error writing to: %s\n", filename);
				errorMain("file failed to open");
			}
            fwrite(data, 1, length, fp);
            fclose(fp);

            //printf("downloaded %d bytes from %s\n", (int)length, task->url);
        }
        else 
        {
            printf("error in response from %s\n", task->url);
        }

    }
    else 
    {
        fprintf(stderr, "error downloading: %s\n", task->url);
    }

    freeTask(task);
}

int main(int argc, char **argv) 
{
    if (argc != 4) 
    {
        fprintf(stderr, "usage: ./downloader url_file num_workers download_dir\n");
        exit(1);
    }
    char *fileURL = argv[1];
    int workerNum = atoi(argv[2]);
    char *downDirec = argv[3];

    createDirectory(downDirec);
    FILE *fp = fopen(fileURL, "r");
    char *line = NULL;
    size_t len = 0;

    if (fp == NULL) 
    {
        exit(EXIT_FAILURE);
    }

    // spawn threads and create work queue(s)
    Context *content = spawnWorker(workerNum);

    int work = 0;
    while ((len = getline(&line, &len, fp)) != -1) 
    {
        if (line[len - 1] == '\n') 
        {
            line[len - 1] = '\0';
        }
        ++work;
        placeQueue(content->toDo, newTask(line));

        // If we've filled the queue up enough, start getting results back
        if (work >= *downDirec) 
        {
            --work;
            taskWaiting(downDirec, content);
        }
    }

    while (work > 0) 
    {
        --work;
        taskWaiting(downDirec, content);
    }
    //cleanup
    fclose(fp);
    free(line);

    freeWorker(content);
    return 0;
}
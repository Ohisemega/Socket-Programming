#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <signal.h>
#include <netdb.h>
#include <string.h>
#include <pthread.h>

#include "query.h"
#define PORT 80
#define BUFSIZE 1024
int checker2 (int func, char * sentence)
{
  if (func == -1)
  {
  puts(sentence);
  exit(EXIT_FAILURE);
  }
}

void error2(char* message)
{
    puts(message);
    exit(EXIT_FAILURE);
}
int ItexClient(char *host, int portNum)
{
    int errorCheck;//integer to check for error values

    char _port[20];
    errorCheck = snprintf(_port, 20, "%d", portNum);
    if(errorCheck >= 20)
    {
       error2("Failed to create socket");
    }

    if(errorCheck<0)
    {
        error2("invalid host name.");
    }

    struct addrinfo iTserver;
    struct addrinfo *match = NULL;

    int itexClient = socket(AF_INET, SOCK_STREAM, 0);
    errorCheck = socket(AF_INET, SOCK_STREAM, 0);
    checker2(errorCheck, "socket: failed");

    memset(&iTserver, 0, sizeof(struct sockaddr_in));

    iTserver.ai_family = AF_INET; //
    iTserver.ai_socktype = SOCK_STREAM; //

    errorCheck = getaddrinfo(host, _port, &iTserver, &match); // get address info assuming correct first result

    if (errorCheck!=0)
    {
        for (int i = 0; i<100; i++)
        {
            errorCheck = getaddrinfo(host, _port, &iTserver, &match);
            if (errorCheck  == 0) break;
            if(i == 99) error("couldn't obtain host name");
        }
    }

    errorCheck = connect(itexClient, match ->ai_addr, match ->ai_addrlen);//connect to the server

    checker2(errorCheck, "Connection error");

    freeaddrinfo(match);
    return itexClient;
}

char *buildAndGetQuery(char* hostName,char *page)
{
    char *request = "GET /%HTTP/1.0\nHost: %s\r\nUser-Agent: getter\r\n\r\n";
    char *query = malloc(strlen(request) + strlen(hostName) + strlen(page));
    int errorCheck3 = sprintf(query,request, page, hostName);
    checker2(errorCheck3, "sprintf: failed");
    return query;
}

//sends a query to the server given from  a connected socket
void sendaQuery(int itexClient, char* query)
{
    int errorCheck2;
   if((errorCheck2 = send(itexClient, query, strlen(query), 0)) < 0) error2("send failed");
}

//creates a new buffer memory
Buffer *nextBuffer()
{
    Buffer *newBuffer = malloc(sizeof(Buffer));
    if (newBuffer == NULL) puts("malloc allocation of memory failed");
    newBuffer ->data = malloc (BUFSIZE);
    if(newBuffer->data == NULL) puts("malloc allocation of buffer->data failed");
    newBuffer->length = 0;
    return newBuffer;
}

//download content from sever connected to the socket intoa buffer memory
Buffer *receiveContent(int itexClient)
{
   Buffer *buffer = nextBuffer();
   int receivedBytes;

   //recieved data from socket into buffer
   while((receivedBytes = recv(itexClient, buffer->data + buffer->length, BUFSIZE, 0)) > 0)
   {
       buffer->length += receivedBytes;
       
       //reallocating the 
       buffer->data = realloc(buffer->data, buffer->length + BUFSIZE);
       if (receivedBytes==-1)
       {
           checker2(receivedBytes, "recv: failed");
       }
   }
//appending the last location of the char array with a \0 value 
//signifying end of string or char input
    buffer->data[buffer->length] = '\0';
    return buffer;
}

//function which utilizes other function to:
//1. get the socket adddress of the connected socket
//2. build a query and return it 
//3. save the content received in a buffer struct
//4. send the query
//5. 
Buffer *httpQuery(char *hostName, char* page, int port)
{
   int itexClient = ItexClient(hostName, port);
   char* query = buildAndGetQuery(hostName, page);
   sendaQuery(itexClient, query);
   free(query);
   Buffer *queryResult = receiveContent(itexClient);
   close(itexClient);
   return queryResult;
}

// split http content from the response string
char *httpGetRequest(Buffer *response) {
    char *headerEnd = strstr(response->data, "\r\n\r\n");

    if (headerEnd) 
    {
        return headerEnd + 4;
    }
    else 
    {
        return response->data;
    }
}

Buffer *httpURL(const char *url)
{
    char hostName[BUFSIZE];
    strncpy(hostName, url,BUFSIZE);
    char *page = strstr(hostName,"/");
    if(page)
    {
        page[0] = '\0';
        ++page;
        return httpQuery(hostName, page, PORT);
    }

    else
    {
        fprintf(stderr,"could not split url into host/page %s\n", url);
        return 0;
    }
}
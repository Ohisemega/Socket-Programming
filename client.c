
// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <signal.h>
#include <string.h>
#include <pthread.h>
#define PORT 80
   
   #if 0
   pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
   #endif

   void sigHandler(int signo)
{
    if (signo == SIGINT)
        printf("!!  OUCH,  CTRL - C received  by server !!\n");
}

int main(int argc, char const *argv[]) 
{ 
    //typedef const struct addr *customStr;
    struct sockaddr_in iTserver; //
    int val = 1;
    int itexClient; //
    char* sentMessage; //
    itexClient = socket(AF_INET, SOCK_STREAM, 0);
    char serverReply [4000];
    size_t totalLength = 0;
    FILE *file;
    int fileLength = 99352;
    //
    if (itexClient == -1)
    {
        printf("Socket wasn't created!\n");
    }
    iTserver.sin_addr.s_addr = inet_addr(""); //
    iTserver.sin_family = AF_INET; //
    iTserver.sin_port = PORT; //

    //
    if(connect(itexClient, (struct sockadddr *) &iTserver, sizeof(iTserver)) != 0)
    {
        printf ("connection error\n");
        exit(1);
    }

    //signal(, sigHandler);
    
    printf("connected");

    //
    sentMessage = "GET/HTTP/1.1\r\n\r\n";//message = "GET /download/pdfurl-guide.pdf HTTP/1.1\r\nHost: www.axmag.com\r\n\r\n Connection: keep-alive\r\n\r\n Keep-Alive: 300\r\n";

    if((send(itexClient, sentMessage, strlen(sentMessage), sizeof(iTserver))) == -1)
    {
        puts("connection error, message not sent!");
    }
    else
    {
        printf("Message delivered!");
    }

    pthread_attr_t attr;//creates a thread attr
    pthread_attr_init(&attr);//
    pthread_attr_setdetachstate(&attr,1);//
    
    //to recieve data
    while (1)
    {

        pthread_create( &threads[socket_index], NULL, forClient, (void*)client_sockfd[socket_index]);
        int fileRec = recv(itexClient, serverReply, sizeof(serverReply), 0);
    
    if(fileRec < 0)
    {
       put("recv failed");
       break;
    }
    else 
    printf("Data receipt Successful");
     totalLength += fileRec;//get the total file length
     fwrite(itexClient, fileRec, sizeof(fileRec), file);
    
    if (totalLength >= fileLength)
    {
        break;
    }
    }

    //alternative data read method
    /*if (read(itexClient, serverReply, sizeof(serverReply)) == 0)
    {
        put("data has been read");
    }*/

//gcc -pthread sourcefilename.c
close(itexClient);
    
}


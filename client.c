
// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h>
#include <pthread.h>
#define PORT 80
   
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
    }

    printf("connected");
    return 0;

    //
    sentMessage = "GET/HTTP/1.1\r\n\r\n";//message = "GET /download/pdfurl-guide.pdf HTTP/1.1\r\nHost: www.axmag.com\r\n\r\n Connection: keep-alive\r\n\r\n Keep-Alive: 300\r\n";

    if(send(itexClient, sentMessage, strlen(sentMessage), sizeof(iTserver)) == -1)
    {
        puts("connection error, message not sent!");
    }
    else
    {
        printf("Message delivered!");
    }
    
    //to recieve data
    while (1)
    {
        int fileRec = recv(itexClient, serverReply, sizeof(serverReply), 0);
    
    if(fileRec < 0)
    {
       put("recv failed");
       break;
    }
    else 
    printf("Data receipt Successful");
     totalLength += fileRec;
     fwrite(itexClient, fileRec, sizeof(fileRec), file);
    
    if (totalLength >= fileLength)
    }
    //alternative data read method
    if (read(itexClient, serverReply, sizeof(serverReply)) == 0)
    {
        put("data has been read");
    }
//gcc -pthread sourcefilename.c
close(itexClient);
    return 0;
} 

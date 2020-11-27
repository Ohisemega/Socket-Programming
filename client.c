
// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
#define PORT 80
   
int main(int argc, char const *argv[]) 
{ 
    //typedef const struct addr *customStr;
    struct sockaddr_in iTserver; //
    int val = 1;
    int itexClient; //
    char* message; //
    itexClient = socket(AF_INET, SOCK_STREAM, 0);
    char serverReply [4000];
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
    message = "GET/HTTP/1.1\r\n\r\n";
    if(send(itexClient, message, strlen(message), sizeof(iTserver)) == -1)
    {
        puts("connection error, message not sent!");
    }
    else
    {
        printf("Data delivered!");
    }
    
    //
    if(recv(itexClient, serverReply, sizeof(serverReply), 0) < 0)
    {
       put("recv failed");
    }
    else 
    printf("Data receipt Successful");

    return 0;
} 


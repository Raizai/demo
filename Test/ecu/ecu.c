#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define DEF_PROTOCOL 0

int readFrom(int fd, char * str){
    long int n;
    do{
        n = read(fd,str,1);
    }while(n > 0 && *str++ != '\0');
    return (n > 0);
}

void readLastLine(int fd){
    char str[200];
    while(readFrom(fd,str));
    printf("%s\n",str);
}

int createSocket(int serverFd, char* socketname){
    struct sockaddr_un serverAddress; //da non assegnare
    struct sockaddr_un clientAddress; //da non assegnare
    struct sockaddr* serverPtr; //puntatore indirizzo server
    struct sockaddr* clientPtr; //puntatore indirizzo client

    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    clientPtr = (struct sockaddr*) &clientAddress; //cast a sockaddr* della dereferenza dell'indirizzo client

    unsigned int serverLen = sizeof(serverAddress);  //dimensione indirizzo server
    unsigned int clientLen = sizeof(clientAddress);  //dimensione indirizzo client

    serverFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);  //inizializzazione del socket
    serverAddress.sun_family = AF_UNIX;                     //assegno al socket la famiglia
    strcpy(serverAddress.sun_path, socketname);     
    unlink(socketname);                            
    bind(serverFd, serverPtr, serverLen);                 
    listen(serverFd, 5);                                    
    return accept(serverFd, clientPtr, &clientLen); 
}

int main(){
    int serverFd, clientFd, throttleFd;

    while(1){
        clientFd = createSocket(serverFd, "../socket");
        if(fork() == 0){
            readLastLine(clientFd);
            close(clientFd);
        }else{
            close(clientFd);
        }
    }
}

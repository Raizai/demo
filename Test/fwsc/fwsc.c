#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#define DEF_PROTOCOL 0

int connectTo(int fd, char* socketName){
    struct sockaddr_un serverAddress; 
    struct sockaddr* serverPtr; 
    serverPtr = (struct sockaddr*) &serverAddress; //cast a sockaddr* della dereferenza dell'indirizzo server
    unsigned int serverLen = sizeof(serverAddress);  //dimensione server
    serverAddress.sun_family = AF_UNIX;  
    strcpy(serverAddress.sun_path,socketName);
    return connect(fd, serverPtr, serverLen); 
}

void run(char* args[]){
    int clientFd = socket(AF_UNIX, SOCK_STREAM, DEF_PROTOCOL);
    char* socketName = "../socket";
    if(connectTo(clientFd,socketName) == -1){
        printf("Errore ... Nuovo tentativo tra 5 secondi\n");
        sleep(5);
    }else{
        char* r = "Hello World";
        write(clientFd,r,strlen(r)+1);
        close(clientFd);
        sleep(5);
        execv("./fwsc",args);
    }
}

int main(int argc, char *argv[]){
    while(1){
        run(argv);
    }
}


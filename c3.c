#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main() {
    int client_socket, serverLen, result, sock;
    struct sockaddr_un serverUNIXAddress;
    struct sockaddr *serverSockAddrPtr;
    char buffer[1024];

    // Create socket
    serverSockAddrPtr = (struct sockaddr *)&serverUNIXAddress;
    serverLen = sizeof(serverUNIXAddress);
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    serverUNIXAddress.sun_family = AF_UNIX;
    strcpy(serverUNIXAddress.sun_path, "demo");

    // Connect to the server
    while (connect(client_socket, serverSockAddrPtr, serverLen) < 0) {
        printf("Connection problem. Try again in 2 sec.\n");
        sleep(2);
    }

    // Receive a string from the server
    //  while ( sock = recv(client_socket, &buffer, sizeof(buffer), 0) < 0)
    //  ;
    //  printf("buf %d\n ",sock );
    if(strcmp(buffer, "Ciao")) { 
        printf("Received from Server: %s\n", buffer);
    } else {
        printf("Non faccio nulla\n");
    }

    // Close the socket
    close(client_socket);

    return 0;
}

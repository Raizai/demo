#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "demo"
#define MAX_CLIENTS 5

void handle_client(int client_socket) {
    // Handle communication with the client here
    // You can use read() and write() to send/receive data
    // Close the client socket when done
    close(client_socket);
    exit(0);
}

int main() {
    int server_socket, client_socket, pid, serverLen, clientLen;
    struct sockaddr_un server_addr, client_addr;
    struct sockaddr_un serverUNIXAddress; /*Server address */
    struct sockaddr* serverSockAddrPtr; /*Ptr to server address*/
    struct sockaddr_un clientUNIXAddress; /*Client address */
    struct sockaddr* clientSockAddrPtr;/*Ptr to client address*/

    socklen_t addr_size = sizeof(client_addr);
    char buffer[] = "Ciao";

    serverSockAddrPtr = (struct sockaddr*) &serverUNIXAddress;
    serverLen = sizeof (serverUNIXAddress);
    clientSockAddrPtr = (struct sockaddr*) &clientUNIXAddress;
    clientLen = sizeof (clientUNIXAddress);

    // Create and initialize the server socket (UNIX domain socket)
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation error");
        exit(1);
    }

    serverUNIXAddress.sun_family = AF_UNIX;
    strcpy(serverUNIXAddress.sun_path, "demo");
    unlink("demo");

    if (bind(server_socket, serverSockAddrPtr, serverLen) < 0) {
        perror("Binding error");
        exit(1);
    }

    listen(server_socket, MAX_CLIENTS);

    printf("Server listening on socket: %s\n", SOCKET_PATH);

    while (1) {
        // Accept incoming client connection
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addr_size);
        
        if ((pid = fork()) == 0) {
            // In child process
            close(server_socket); // Close the server socket in the child
            send(client_socket, buffer, strlen(buffer), 0);
            exit(0);
        } else if (pid > 0) {

            close(client_socket); // Close the client socket in the parent
        } else {
            perror("Error forking process");
        }
    }
    close(server_socket);
    return 0;
}

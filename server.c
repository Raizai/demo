#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_CLIENTS 2

int main() {
    int server_fd, new_socket, client_sockets[MAX_CLIENTS];
    struct sockaddr_in server_addr, new_addr;
    socklen_t addr_size;
    char buffer[1024];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation error");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = PORT;
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Binding error");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == 0) {
        printf("Listening...\n");
    } else {
        perror("Listening error");
        exit(1);
    }

    addr_size = sizeof(new_addr);

    // Accept incoming connections

    // Receive a string from one client and send it to the other
    while (1) {
        for (int i = 0; i < MAX_CLIENTS; i++) {
            new_socket = accept(server_fd, (struct sockaddr *)&new_addr, &addr_size);
            client_sockets[i] = new_socket;
            printf("Client %d connected\n", i + 1);
        }

        int sender, receiver;
        // Assume client 1 sends to client 2, change this logic as needed
        sender = 0;
        receiver = 1;

        // Receive message from sender client
        recv(client_sockets[sender], buffer, sizeof(buffer), 0);
        printf("Received from Client %d: %s\n", sender + 1, buffer);

        // Send the message to the receiver client
        send(client_sockets[receiver], buffer, strlen(buffer), 0);
    }

    return 0;
}

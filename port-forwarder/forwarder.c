//
// Created by Andres Ortiz Osorio on 11/6/26.
//

#include "forwarder.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread/pthread.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define IP_TYPE AF_INET
#define PORT 8080
#define BUFFER_SIZE 1024

int create_client_fd(int fd);
void *handle_client_loop(void *args);

typedef struct {
    int fd;
} client_args;

int main() {

    // Define IPv4 address and port we will listen to

    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    const socklen_t addr_len = sizeof(addr);
    addr.sin_family = IP_TYPE;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // TCP connection-setup

    // Create socket
    const int server_socket_fd = socket(IP_TYPE, SOCK_STREAM, 0);
    if (server_socket_fd == -1) { perror("socket"); exit(EXIT_FAILURE); }
    printf("Socket created\n");

    // Bind socket to server IP & Port
    if (bind(server_socket_fd, (struct sockaddr *)&addr, addr_len) == -1) { perror("bind"); exit(EXIT_FAILURE); }
    printf("Socket bound\n");

    // Listen for connections
    if (listen(server_socket_fd, 10) == -1) { perror("listen"); exit(EXIT_FAILURE); }
    printf("Server listening on port %d \n", PORT);

    // Run thread-loop

    pthread_t thread_set[10];

    int i = 0;
    while (i < 10) {
        const int client_socket_fd = create_client_fd(server_socket_fd);
        client_args *args = malloc(sizeof(client_args));
        args->fd = client_socket_fd;
        pthread_create(&thread_set[i], NULL, handle_client_loop, args);
        i++;
    }

    for (int i = 0; i < 10; i++) {
        pthread_join(thread_set[i], NULL);
    }

    close(server_socket_fd);
}

int create_client_fd(int fd) {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    const int client_socket_fd = accept(fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_socket_fd == -1) { perror("accept"); exit(EXIT_FAILURE); }
    return client_socket_fd;
}

void *handle_client_loop(void *args) {
    client_args *a = args;
    const int fd = a->fd;
    free(a);

    printf("Client connected\n");
    while (1) {

        // Create as many threads as there are client-connections

        char read_buffer [BUFFER_SIZE];

        // Receive and write messages
        const ssize_t n = recv(fd, read_buffer, BUFFER_SIZE - 1, 0);
        if (n == -1) { perror("recv"); exit(EXIT_FAILURE); }
        if (n == 0) { break; }
        read_buffer[n] = '\0';

        // Log contents
        printf("Client sent: %s", read_buffer);

        // Relay back contents to the client
        char write_buffer[n + 100];
        const int len = snprintf(write_buffer, sizeof(write_buffer), "Server Relayed: %s", read_buffer);
        if (send(fd, write_buffer, len, 0) == -1) { perror("send"); exit(EXIT_FAILURE); }
    }

    close(fd);
    return NULL;
}
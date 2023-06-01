#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define ERROR           (-1)
#define BUFFER_SIZE     1024

int main(int argc, char **argv) {
    int ret;
    int client_sock;
    int server_port;
    char *server_ip;
    char *server_port_str;
    char buffer[BUFFER_SIZE] = {};
    socklen_t sock_addr_length = sizeof(struct sockaddr_in);
    struct sockaddr_in server_sock_addr;

    // Get server settings
    server_ip = getenv("SERVER_IP");
    if (server_ip == NULL) {
        fprintf(stderr, "getenv : set environment variable SERVER_IP\n");
        return EXIT_FAILURE;
    }

    server_port_str = getenv("SERVER_PORT");
    if (server_port_str == NULL) {
        fprintf(stderr, "getenv : set environment variable SERVER_PORT\n");
        return EXIT_FAILURE;
    }
    server_port = atoi(server_port_str);

    // Create client socket
    client_sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client_sock == ERROR) {
        perror("socket");
        close(client_sock);
        return EXIT_FAILURE;
    }

    // Initialize server address
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = server_port;
    ret = inet_pton(AF_INET, server_ip, &(server_sock_addr.sin_addr));
    if (ret == ERROR) {
        perror("inet_pton");
        close(client_sock);
        return EXIT_FAILURE;
    }

    // Start communication
    while (true) {
        printf("Enter message: ");
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        if (buffer[strlen(buffer) - 1] == '\n') {
            buffer[strlen(buffer) - 1] = '\0';
        }

        ret = sendto(client_sock, buffer, strlen(buffer), 0, (struct sockaddr *) &server_sock_addr,
                     sock_addr_length);
        if (ret == ERROR) {
            perror("sendto");
            continue;
        }

        ret = recvfrom(client_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &server_sock_addr,
                       &sock_addr_length);
        if (ret == ERROR) {
            perror("recvfrom");
            continue;
        }
        printf("Message received: %s\n\n", buffer);
    }

    close(client_sock);
    return EXIT_SUCCESS;
}
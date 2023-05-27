#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define ERROR                   (-1)
#define BUFFER_SIZE             1024
#define SOCK_FILE_NAME_LENGTH   108

const char *SERVER_SOCK_PATH = "server-udp-echo.sock";

void generate_sock_file_name(char *sock_file_name);

int main(int argc, char **argv) {
    int ret;
    int client_sock;
    char client_sock_path[SOCK_FILE_NAME_LENGTH] = {};
    char buffer[BUFFER_SIZE] = {};
    struct sockaddr_un server_sock_addr;
    struct sockaddr_un client_sock_addr;
    socklen_t sock_addr_length = sizeof(struct sockaddr_un);

    // Create client socket
    generate_sock_file_name(client_sock_path);

    client_sock_addr.sun_family = AF_UNIX;
    strncpy(client_sock_addr.sun_path, client_sock_path, SOCK_FILE_NAME_LENGTH - 1);

    client_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (client_sock == ERROR) {
        close(client_sock);
        perror("socket");
        return ERROR;
    }

    ret = bind(client_sock, (struct sockaddr *) &client_sock_addr, sock_addr_length);
    if (ret == ERROR) {
        close(client_sock);
        perror("bind");
        return EXIT_FAILURE;
    }

    // Initialize server address
    server_sock_addr.sun_family = AF_UNIX;
    strncpy(server_sock_addr.sun_path, SERVER_SOCK_PATH, SOCK_FILE_NAME_LENGTH - 1);

    // Start communication
    while (true) {
        printf("Enter message: ");
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

void generate_sock_file_name(char *sock_file_name) {
    memset(sock_file_name, 0, SOCK_FILE_NAME_LENGTH);
    snprintf(sock_file_name, SOCK_FILE_NAME_LENGTH, "client-udp-echo-%d.sock", getpid());
}
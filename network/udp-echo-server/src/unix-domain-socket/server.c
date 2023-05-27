#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define ERROR           (-1)
#define BUFFER_SIZE     1024

const char *SERVER_SOCK_PATH = "server-udp-echo.sock";

int main(int argc, char **argv) {
    int ret;
    int server_sock;
    char buffer[BUFFER_SIZE] = {};
    struct sockaddr_un server_sock_addr;
    struct sockaddr_un client_sock_addr;
    socklen_t sock_addr_length = sizeof(struct sockaddr_un);

    // Create server socket
    server_sock_addr.sun_family = AF_UNIX;
    strncpy(server_sock_addr.sun_path, SERVER_SOCK_PATH, sizeof(server_sock_addr.sun_path) - 1);

    server_sock = socket(AF_UNIX, SOCK_DGRAM, 0);
    if (server_sock == ERROR) {
        close(server_sock);
        perror("socket");
        return EXIT_FAILURE;
    }

    ret = bind(server_sock, (struct sockaddr *) &server_sock_addr, sock_addr_length);
    if (ret == ERROR) {
        close(server_sock);
        perror("bind");
        return ERROR;
    }

    // Start communication
    while (true) {
        ret = recvfrom(server_sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &client_sock_addr,
                       &sock_addr_length);
        if (ret == ERROR) {
            perror("recvfrom");
            continue;
        }
        printf("Message received from %s\n", client_sock_addr.sun_path);
        printf("Message content: %s\n\n", buffer);

        ret = sendto(server_sock, buffer, strlen(buffer), 0, (struct sockaddr *) &client_sock_addr,
                     sock_addr_length);
        if (ret == ERROR) {
            perror("sendto");
            continue;
        }
    }

    close(server_sock);

    return EXIT_SUCCESS;
}

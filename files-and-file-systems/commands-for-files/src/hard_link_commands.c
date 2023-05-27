#include "commands.h"
#include <stdio.h>
#include <unistd.h>

int create_hard_link(int argc, char **argv) {
    if (argc < 2) {
        printf("USAGE: create_hard_link <target_path> <link_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *target_path = argv[0];
    char *link_path = argv[1];

    ret = link(target_path, link_path);
    if (ret == ERROR) {
        fprintf(stderr, "%s, %s", target_path, link_path);
        perror("");
        return ERROR;
    }

    return SUCCESS;
}

int remove_hard_link(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: remove_hard_link <file_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *file_path = argv[0];

    ret = unlink(file_path);
    if (ret == ERROR) {
        perror(file_path);
        return ERROR;
    }

    return SUCCESS;
}
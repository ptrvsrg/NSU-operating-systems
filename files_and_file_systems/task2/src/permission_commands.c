#include "commands.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int display_file_permissions(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: display_file_permissions <file_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *file_path = argv[0];
    struct stat file_info;

    ret = stat(file_path, &file_info);
    if (ret == ERROR) {
        perror(file_path);
        return ERROR;
    }

    printf("Permissions: %o\n", file_info.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
    printf("Hard links: %ld\n", file_info.st_nlink);

    return SUCCESS;
}

static mode_t chmod_from_string(const char* mode_str) {
    char* endptr = NULL;
    mode_t permissions = strtol(mode_str, &endptr, 8);

    if (endptr == mode_str || *endptr != '\0'|| permissions > 0777) {
        fprintf(stderr, "%s: invalid mode\n", mode_str);
        return ERROR;
    }

    return permissions;
}

int change_file_permissions(int argc, char **argv) {
    if (argc < 2) {
        printf("USAGE: change_file_permissions <file_path> <permissions>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *file_path = argv[0];
    mode_t permissions = chmod_from_string(argv[1]);

    ret = chmod(file_path, permissions);
    if (ret == ERROR) {
        perror(file_path);
        return ERROR;
    }

    return SUCCESS;
}
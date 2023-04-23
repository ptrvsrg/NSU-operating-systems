#include "commands.h"
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int create_dir(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: create_dir <dir_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *dir_path = argv[0];

    ret = mkdir(dir_path, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IXOTH);
    if (ret == ERROR) {
        perror(dir_path);
        return ERROR;
    }

    return SUCCESS;
}

int list_dir_content(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: list_dir_content <dir_path>\n");
        return ERROR;
    }

    char *dir_path = argv[0];
    DIR *dir = NULL;
    struct dirent *dir_entry = NULL;

    dir = opendir(dir_path);
    if (dir == NULL) {
        perror(dir_path);
        return ERROR;
    }

    dir_entry = readdir(dir);
    while (dir_entry != NULL) {
        printf("%s\n", dir_entry->d_name);
        dir_entry = readdir(dir);
    }

    closedir(dir);
    return SUCCESS;
}

int remove_dir(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: remove_dir <dir_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *dir_path = argv[0];

    ret = rmdir(dir_path);
    if (ret == ERROR) {
        perror(dir_path);
        return ERROR;
    }

    return SUCCESS;
}
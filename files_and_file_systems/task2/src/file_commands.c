#include "commands.h"
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

int create_file(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: create_file <file_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *file_path = argv[0];

    ret = creat(file_path, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH);
    if (ret == ERROR) {
        perror(file_path);
        return ERROR;
    }

    return SUCCESS;
}

int display_file_content(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: display_file_content <file_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    int read = 0;
    char *file_path = argv[0];
    char buffer[BUFFER_SIZE] = {};
    FILE *file_stream = NULL;

    file_stream = fopen(file_path, "r");
    if (file_stream == NULL) {
        perror(file_path);
        return ERROR;
    }

    read = fread(buffer, sizeof(char), BUFFER_SIZE, file_stream);
    while (read > 0) {
        puts(buffer);
        read = fread(buffer, sizeof(char), BUFFER_SIZE, file_stream);
    }

    ret = ferror(file_stream);
    if (ret != SUCCESS) {
        perror(file_path);
        ret = ERROR;
    }

    fclose(file_stream);

    return ret;
}

int remove_file(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: remove_file <file_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    ret = remove_hard_link(argc, argv);
    return ret;
}
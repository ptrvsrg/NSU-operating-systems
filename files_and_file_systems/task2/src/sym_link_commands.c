#include "commands.h"
#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>

int create_sym_link(int argc, char **argv) {
    if (argc < 2) {
        printf("USAGE: create_sym_link <target_path> <link_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *target_path = argv[0];
    char *link_path = argv[1];

    ret = symlink(target_path, link_path);
    if (ret == ERROR) {
        fprintf(stderr, "%s, %s", target_path, link_path);
        perror("");
        return ERROR;
    }

    return SUCCESS;
}

int display_sym_link_content(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: display_sym_link_content <link_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *link_path = argv[0];
    char target_path[PATH_MAX] = {};

    ret = readlink(link_path, target_path, BUFFER_SIZE);
    if (ret == ERROR) {
        perror(link_path);
        return ERROR;
    }

    printf("%s\n", target_path);

    return SUCCESS;
}

int display_file_content_by_sym_link(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: display_file_content_by_sym_link <link_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *link_path = argv[0];
    char target_path[PATH_MAX] = {};
    char *new_argv[2] = {};

    ret = readlink(link_path, target_path, BUFFER_SIZE);
    if (ret == ERROR) {
        perror(link_path);
        return ERROR;
    }

    new_argv[0] = target_path;
    ret = display_file_content(1, new_argv);
    return ret;
}

int remove_sym_link(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: remove_symbolic_link <file_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *link_path = argv[0];
    struct stat file_info;

    ret = stat(link_path, &file_info);
    if (ret == ERROR) {
        perror(link_path);
        return ERROR;
    }

    if (file_info.st_mode != S_IFLNK) {
        fprintf(stderr, "%s: file is not symbolic link\n", link_path);
        return ERROR;
    }

    ret = remove_hard_link(argc, argv);
    return ret;
}
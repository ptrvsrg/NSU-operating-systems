#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "commands.h"

int process_command_line_args(int argc, char **argv);

int main(int argc, char **argv) {
    int ret = SUCCESS;

    ret = process_command_line_args(argc, argv);
    if (ret == ERROR) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int process_command_line_args(int argc, char **argv) {
    char *program_name = NULL;

    program_name = strrchr(argv[0], '/');
    if (program_name == NULL) {
        program_name = argv[0];
    } else {
        program_name++;
    }

    if (strcmp(program_name, "change_file_permissions") == 0) {
        change_file_permissions(argc - 1, argv + 1);
    } else if (strcmp(program_name, "create_dir") == 0) {
        create_dir(argc - 1, argv + 1);
    } else if (strcmp(program_name, "create_file") == 0) {
        create_file(argc - 1, argv + 1);
    } else if (strcmp(program_name, "create_hard_link") == 0) {
        create_hard_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "create_sym_link") == 0) {
        create_sym_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "display_file_content_by_sym_link") == 0) {
        display_file_content_by_sym_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "display_file_content") == 0) {
        display_file_content(argc - 1, argv + 1);
    } else if (strcmp(program_name, "display_file_permissions") == 0) {
        display_file_permissions(argc - 1, argv + 1);
    } else if (strcmp(program_name, "display_sym_link_content") == 0) {
        display_sym_link_content(argc - 1, argv + 1);
    } else if (strcmp(program_name, "list_dir_content") == 0) {
        list_dir_content(argc - 1, argv + 1);
    } else if (strcmp(program_name, "remove_dir") == 0) {
        remove_dir(argc - 1, argv + 1);
    } else if (strcmp(program_name, "remove_file") == 0) {
        remove_file(argc - 1, argv + 1);
    } else if (strcmp(program_name, "remove_hard_link") == 0) {
        remove_hard_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "remove_sym_link") == 0) {
        remove_sym_link(argc - 1, argv + 1);
    } else {
        printf("Unknown command '%s'\n", program_name);
        return ERROR;
    }

    return SUCCESS;
}
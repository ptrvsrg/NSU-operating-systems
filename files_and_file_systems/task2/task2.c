#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 256
#define SUCCESS 0
#define ERROR (-1)

int process_command_line_args(int argc, char **argv);
int create_dir(int argc, char **argv);
int list_dir_content(int argc, char **argv);
int remove_dir(int argc, char **argv);
int create_file(int argc, char **argv);
int display_file_content(int argc, char **argv);
int remove_file(int argc, char **argv);
int create_symbolic_link(int argc, char **argv);
int display_symbolic_link(int argc, char **argv);
int display_file_by_symbolic_link(int argc, char **argv);
int remove_symbolic_link(int argc, char **argv);
int create_hard_link(int argc, char **argv);
int remove_hard_link(int argc, char **argv);
int display_file_permissions(int argc, char **argv);
int change_file_permissions(int argc, char **argv);

int main(int argc, char **argv) {
    int ret = SUCCESS;

    ret = process_command_line_args(argc, argv);
    if (ret == ERROR) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void print_usage(char *program_name) {
    printf("USAGE: %s <option> <arg>\n", program_name);
}

int process_command_line_args(int argc, char **argv) {
    char *program_name = NULL;

    program_name = strrchr(argv[0], '/');
    if (program_name == NULL) {
        program_name = argv[0];
    } else {
        ++program_name;
    }

    program_name = "change_file_permissions";

    if (strcmp(program_name, "create_dir") == 0) {
        create_dir(argc - 1, argv + 1);
    } else if (strcmp(program_name, "list_dir_content") == 0) {
        list_dir_content(argc - 1, argv + 1);
    } else if (strcmp(program_name, "remove_dir") == 0) {
        remove_dir(argc - 1, argv + 1);
    } else if (strcmp(program_name, "create_file") == 0) {
        create_file(argc - 1, argv + 1);
    } else if (strcmp(program_name, "display_file_content") == 0) {
        display_file_content(argc - 1, argv + 1);
    } else if (strcmp(program_name, "remove_file") == 0) {
        remove_file(argc - 1, argv + 1);
    } else if (strcmp(program_name, "create_symbolic_link") == 0) {
        create_symbolic_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "display_symbolic_link") == 0) {
        display_symbolic_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "display_file_by_symbolic_link") == 0) {
        display_file_by_symbolic_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "remove_symbolic_link") == 0) {
        remove_symbolic_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "create_hard_link") == 0) {
        create_hard_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "remove_hard_link") == 0) {
        remove_hard_link(argc - 1, argv + 1);
    } else if (strcmp(program_name, "display_file_permissions") == 0) {
        display_file_permissions(argc - 1, argv + 1);
    } else if (strcmp(program_name, "change_file_permissions") == 0) {
        change_file_permissions(argc - 1, argv + 1);
    } else {
        printf("Unknown command '%s'\n", program_name);
        return ERROR;
    }

    return SUCCESS;
}

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

    char *file_path = argv[0];
    char buffer[BUFFER_SIZE] = {};
    FILE *file_stream = NULL;

    file_stream = fopen(file_path, "r");
    if (file_stream == NULL) {
        perror(file_path);
        return ERROR;
    }
    // TODO: переписать через fread
    while (fgets(buffer, BUFFER_SIZE, file_stream) != NULL) {
        printf("%s", buffer);
    }
    fclose(file_stream);

    return SUCCESS;
}

int remove_file(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: remove_file <file_path>\n");
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

int create_symbolic_link(int argc, char **argv) {
    if (argc < 2) {
        printf("USAGE: create_symbolic_link <target_path> <link_path>\n");
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

int display_symbolic_link(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: display_symbolic_link <link_path>\n");
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

    return SUCCESS;
}

int display_file_by_symbolic_link(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: display_symbolic_link <link_path>\n");
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

    ret = display_file_content(1, &target_path);
    return ret;
}

int remove_symbolic_link(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: remove_symbolic_link <file_path>\n");
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

// TODO: такая же как remove_file
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

int display_file_permissions(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: display_file_permissions <file_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *file_path = argv[0];
    struct stat *file_info = NULL;

    ret = stat(file_path, file_info);
    if (ret == ERROR) {
        perror(file_path);
        return ERROR;
    }

    printf("File '%s' has permissions %o and %ld hard links\n",
           file_path,
           file_info->st_mode & S_IRWXU | S_IRWXG | S_IRWXO,
           file_info->st_nlink);

    return SUCCESS;
}

int change_file_permissions(int argc, char **argv) {
    if (argc < 1) {
        printf("USAGE: change_file_permissions <file_path>\n");
        return ERROR;
    }

    int ret = SUCCESS;
    char *file_path = argv[0];
    mode_t permissions = strtol(argv[1], NULL, 8);

    ret = chmod(file_path, permissions);
    if (ret == ERROR) {
        perror(file_path);
        return ERROR;
    }

    return SUCCESS;
}
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SUCCESS 0
#define ERROR (-1)
#define BUFFER_MAX 256

long max(long a, long b);
void print_usage(const char *program_name);
void reverse_string(char *dest, const char *src, int dest_length);
void get_file_name(const char *file_path, char *file_name);
void join_file_path(const char *working_dir, const char *file_name, char *file_path);
int process_command_line_args(int argc, char **argv);
int create_reverse_dir(const char *dir_path, const char *reverse_dir_path);
int process_dir_entry(const struct dirent *dir_entry, const char *dest_working_dir, const char *src_working_dir);
long get_file_size(FILE *file_stream);
int read_file(char *buffer, size_t size, FILE *in_stream);
int write_file(const char *buffer, size_t size, FILE *in_stream);
int create_reverse_file(const char *file_path, const char *reverse_file_path);

int main(int argc, char **argv) {
    int ret = SUCCESS;

    if (argc < 2) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    ret = process_command_line_args(argc, argv);
    if (ret == ERROR)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

long max(long a, long b) {
    return (a <= b) ? b : a;
}

void print_usage(const char *program_name) {
    printf("Usage: %s dir...\n", program_name);
}

void reverse_string(char *dest, const char *src, int dest_length) {
    char temp;
    char *begin = NULL;
    char *end = NULL;

    strncpy(dest, src, dest_length);
    begin = dest;
    end = dest + dest_length - 1;

    while (end > begin) {
        temp = *end;
        *end = *begin;
        *begin = temp;
        ++begin;
        --end;
    }
}

void get_file_name(const char *file_path, char *file_name) {
    char *last_slash = strrchr(file_path, '/');

    if (last_slash == NULL)
        strncpy(file_name, file_path, NAME_MAX);
    else
        strncpy(file_name, last_slash + 1, NAME_MAX);
}

void join_file_path(const char *working_dir, const char *file_name, char *file_path) {
    snprintf(file_path, PATH_MAX, "%s/%s", working_dir, file_name);
}

int process_command_line_args(int argc, char **argv) {
    int ret = SUCCESS;
    char dir_name[NAME_MAX + 1] = {};
    char reverse_dir_name[NAME_MAX + 1] = {};
    char reverse_dir_path[PATH_MAX + 1] = {};

    for (int i = 1; i < argc; ++i) {
        get_file_name(argv[i], dir_name);
        reverse_string(reverse_dir_name, dir_name, strlen(dir_name));
        join_file_path(".", reverse_dir_name, reverse_dir_path);

        ret = create_reverse_dir(argv[i], reverse_dir_path);
        if (ret == ERROR)
            return ERROR;
    }

    return SUCCESS;
}

int create_reverse_dir(const char *dir_path, const char *reverse_dir_path) {
    int ret = SUCCESS;
    DIR *dir_stream = NULL;
    struct dirent *dir_entry = NULL;

    dir_stream = opendir(dir_path);
    if (dir_stream == NULL) {
        perror(dir_path);
        return ERROR;
    }

    ret = mkdir(reverse_dir_path, S_IRWXU | S_IRGRP | S_IWGRP | S_IROTH | S_IXOTH);
    if (ret == ERROR) {
        perror(reverse_dir_path);
        return ERROR;
    }

    dir_entry = readdir(dir_stream);
    while (dir_entry != NULL) {
        ret = process_dir_entry(dir_entry, dir_path, reverse_dir_path);
        if (ret == ERROR) {
            closedir(dir_stream);
            return ERROR;
        }

        dir_entry = readdir(dir_stream);
    }

    closedir(dir_stream);
    return SUCCESS;
}

int process_dir_entry(const struct dirent *dir_entry, const char *dest_working_dir, const char *src_working_dir) {
    int ret = SUCCESS;
    char file_path[PATH_MAX + 1] = {};
    char reverse_file_name[NAME_MAX + 1] = {};
    char reverse_file_path[PATH_MAX + 1] = {};

    join_file_path(dest_working_dir, dir_entry->d_name, file_path);
    reverse_string(reverse_file_name, dir_entry->d_name, strlen(dir_entry->d_name));
    join_file_path(src_working_dir, reverse_file_name, reverse_file_path);

    if (S_ISREG(dir_entry->d_type))
        ret = create_reverse_file(file_path, reverse_file_path);
    else if (S_ISDIR(dir_entry->d_type) && strcmp(dir_entry->d_name, ".") != 0 && strcmp(dir_entry->d_name, "..") != 0)
        ret = create_reverse_dir(file_path, reverse_file_path);

    return ret;
}

long get_file_size(FILE *file_stream) {
    int ret = SUCCESS;
    long begin_pos = 0;
    long size = 0;

    begin_pos = ftell(file_stream);
    if (begin_pos == ERROR)
        return ERROR;

    ret = fseek(file_stream, 0, SEEK_END);
    if (ret == ERROR)
        return ERROR;

    size = ftell(file_stream);
    if (size == ERROR)
        return ERROR;

    ret = fseek(file_stream, begin_pos, SEEK_SET);
    if (ret == ERROR)
        return ERROR;

    return size;
}

int read_file(char *buffer, size_t size, FILE *in_stream) {
    int ret = SUCCESS;
    size_t read = 0;
    size_t total_read = 0;

    while (total_read < size) {
        read = fread(buffer + total_read, sizeof(char), size - total_read, in_stream);

        ret = ferror(in_stream);
        if (read == 0 && ret != SUCCESS)
            return ERROR;

        total_read += read;
    }

    return SUCCESS;
}

int write_file(const char *buffer, size_t size, FILE *in_stream) {
    int ret = SUCCESS;
    size_t written = 0;
    size_t total_written = 0;

    while (total_written < size) {
        written = fwrite(buffer + total_written, sizeof(char), size - total_written, in_stream);

        ret = ferror(in_stream);
        if (written == 0 && ret != SUCCESS)
            return ERROR;

        total_written += written;
    }

    return SUCCESS;
}

int create_reverse_file(const char *file_path, const char *reverse_file_path) {
    int ret = SUCCESS;
    size_t reversed = 0;
    size_t file_size = 0;
    char buffer[BUFFER_MAX + 1] = {};
    FILE *src_file_stream = NULL;
    FILE *dest_file_stream = NULL;

    src_file_stream = fopen(file_path, "r");
    if (src_file_stream == NULL) {
        perror(file_path);
        return ERROR;
    }

    dest_file_stream = fopen(reverse_file_path, "w");
    if (dest_file_stream == NULL) {
        perror(reverse_file_path);
        fclose(src_file_stream);
        return ERROR;
    }

    file_size = get_file_size(src_file_stream);
    if (file_size == ERROR) {
        perror(file_path);
        fclose(dest_file_stream);
        fclose(src_file_stream);
        return ERROR;
    }

    while (reversed != file_size) {
        long offset = max(0, file_size - reversed - BUFFER_MAX);
        size_t count = file_size - offset - reversed;

        ret = fseek(src_file_stream, offset, SEEK_SET);
        if (ret == ERROR) {
            perror(file_path);
            break;
        }

        memset(buffer, 0, BUFFER_MAX);
        ret = read_file(buffer, count, src_file_stream);
        if (ret == ERROR) {
            perror(file_path);
            break;
        }

        reverse_string(buffer, buffer, count);
        ret = write_file(buffer, count, dest_file_stream);
        if (ret == ERROR) {
            perror(reverse_file_path);
            break;
        }

        reversed += count;
    }

    fclose(dest_file_stream);
    fclose(src_file_stream);
    return ret;
}

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define SUCCESS 0
#define ERROR (-1)
#define BUFFER_MAX 256

void print_error(const char *file_path);
long max(long x, long y);
void reverse_string(char *dest, const char *src);
long get_file_size(FILE *file);
int read_file(char *buffer, size_t size, FILE *in_stream);
int write_file(const char *buffer, size_t size, FILE *in_stream);
int create_reverse_dir(const char *working_dir, const char *dir_name);
int create_reverse_file(const char *dest_working_dir, const char *src_working_dir, const char *file_name);

int main(int argc, char **argv) {
    char working_dir[PATH_MAX] = {};
    char dir_name[NAME_MAX] = {};
    char *last_slash = NULL;

    if (argc < 2) {
        printf("Usage: %s DIRECTORY...\n", argv[0]);
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i) {
        last_slash = strrchr(argv[i], '/');
        if (last_slash != NULL) {
            strncpy(working_dir, argv[i], last_slash - argv[i] + 1);
            strcpy(dir_name, last_slash + 1);
        } else {
            strcpy(working_dir, "./");
            strcpy(dir_name, argv[i]);
        }

        if (create_reverse_dir(working_dir, dir_name) == ERROR)
            return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void print_error(const char *file_path) {
    fprintf(stderr, "%s : %s\n", file_path, strerror(errno));
}

long max(long x, long y) {
    return (x <= y) ? y : x;
}

void reverse_string(char *dest, const char *src) {
    char temp;
    char *begin = NULL;
    char *end = NULL;

    strcpy(dest, src);
    begin = dest;
    end = dest + strlen(dest) - 1;

    while (end > begin) {
        temp = *end;
        *end = *begin;
        *begin = temp;
        ++begin;
        --end;
    }
}

long get_file_size(FILE *file) {
    long begin_pos = 0;
    long size = 0;

    if ((begin_pos = ftell(file)) == ERROR)
        return ERROR;

    if (fseek(file, 0, SEEK_END) == ERROR)
        return ERROR;

    if ((size = ftell(file)) == ERROR)
        return ERROR;

    if (fseek(file, begin_pos, SEEK_SET) == ERROR)
        return ERROR;

    return size;
}

int read_file(char *buffer, size_t size, FILE *in_stream) {
    size_t read = 0;
    size_t total_read = 0;

    while (total_read < size) {
        read = fread(buffer + total_read, sizeof(char), size - total_read, in_stream);

        if (read == 0 && ferror(in_stream) != SUCCESS)
            return ERROR;

        total_read += read;
    }

    return SUCCESS;
}

int write_file(const char *buffer, size_t size, FILE *in_stream) {
    size_t written = 0;
    size_t total_written = 0;

    while (total_written < size) {
        written = fwrite(buffer + total_written, sizeof(char), size - total_written, in_stream);

        if (written == 0 && ferror(in_stream) != SUCCESS)
            return ERROR;

        total_written += written;
    }

    return SUCCESS;
}

int create_reverse_dir(const char *working_dir, const char *dir_name) {
    char dir_path[PATH_MAX] = {};
    char reverse_dir_name[NAME_MAX] = {};
    char reverse_dir_path[PATH_MAX] = {};
    DIR *dir = NULL;
    struct dirent *entry = NULL;

    sprintf(dir_path, "%s%s", working_dir, dir_name);

    reverse_string(reverse_dir_name, dir_name);
    sprintf(reverse_dir_path, "%s%s", working_dir, reverse_dir_name);

    dir = opendir(dir_path);
    if (dir == NULL) {
        print_error(dir_path);
        return ERROR;
    }

    if (mkdir(reverse_dir_path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == ERROR) {
        print_error(reverse_dir_path);
        return ERROR;
    }

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_REG) {
            if (create_reverse_file(dir_path, reverse_dir_path, entry->d_name) == ERROR) {
                closedir(dir);
                return ERROR;
            }
        }
    }

    closedir(dir);
    return SUCCESS;
}

int create_reverse_file(const char *dest_working_dir, const char *src_working_dir, const char *file_name) {
    size_t reversed = 0;
    size_t file_size = 0;
    char file_path[PATH_MAX] = {};
    char reverse_file_path[PATH_MAX] = {};
    char reverse_file_name[NAME_MAX] = {};
    char buffer[BUFFER_MAX] = {};
    FILE *src_stream = NULL;
    FILE *dest_stream = NULL;

    sprintf(file_path, "%s/%s", dest_working_dir, file_name);

    reverse_string(reverse_file_name, file_name);
    sprintf(reverse_file_path, "%s/%s", src_working_dir, reverse_file_name);

    src_stream = fopen(file_path, "rb");
    if (src_stream == NULL) {
        print_error(file_path);
        return ERROR;
    }

    dest_stream = fopen(reverse_file_path, "wb");
    if (dest_stream == NULL) {
        fclose(src_stream);
        print_error(reverse_file_path);
        return ERROR;
    }

    file_size = get_file_size(src_stream);
    if (file_size == ERROR) {
        fclose(src_stream);
        fclose(dest_stream);
        print_error(file_path);
        return ERROR;
    }

    while (reversed != file_size) {
        long offset = max(0, file_size - reversed - BUFFER_MAX);
        size_t count = file_size - offset - reversed;

        if (fseek(src_stream, offset, SEEK_SET) == ERROR) {
            fclose(src_stream);
            fclose(dest_stream);
            print_error(file_path);
            return ERROR;
        }

        memset(buffer, 0, BUFFER_MAX);

        if (read_file(buffer, count, src_stream) == ERROR) {
            fclose(src_stream);
            fclose(dest_stream);
            print_error(file_path);
            return ERROR;
        }

        reverse_string(buffer, buffer);

        if (write_file(buffer, count, dest_stream) == ERROR) {
            fclose(src_stream);
            fclose(dest_stream);
            print_error(reverse_file_path);
            return ERROR;
        }

        reversed += count;
    }

    fclose(src_stream);
    fclose(dest_stream);

    return SUCCESS;
}

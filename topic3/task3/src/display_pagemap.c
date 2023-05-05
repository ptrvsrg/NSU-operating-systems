#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 0
#define ERROR (-1)
#define BUFFER_SIZE 1024
#define PAGE_SIZE 4096
#define PFN_MASK (((uint64_t)1 << 55) - 1)
#define PAGE_PRESENT ((uint64_t)1 << 63)

int print_pagemap();
int extract_number(const char* line, uint64_t *value, char **end_ptr);
int parse_maps_entry(const char *maps_entry, uint64_t *start_addr, uint64_t *end_addr);
int print_pagemap_entries(int pagemap_fd, uint64_t start_addr, uint64_t end_addr);

int main() {
    int ret = print_pagemap();

    if (ret == ERROR) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int print_pagemap() {
    int ret = SUCCESS;
    int pagemap_fd = 0;
    char *pagemap_file = "/proc/self/pagemap";
    char *maps_file = "/proc/self/maps";
    FILE *maps_fp = NULL;

    maps_fp = fopen(maps_file, "r");
    if (maps_fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    pagemap_fd = open(pagemap_file, O_RDONLY);
    if (pagemap_fd == ERROR) {
        perror("open");
        return EXIT_FAILURE;
    }

    while (true) {
        uint64_t start_addr;
        uint64_t end_addr;
        char maps_entry[BUFFER_SIZE] = {};
        char *ptr_ret = NULL;

        ptr_ret = fgets(maps_entry, BUFFER_SIZE, maps_fp);
        if (ptr_ret == NULL) {
            ret = ferror(maps_fp);
            if (ret != SUCCESS) {
                perror("fscanf");
                ret = ERROR;
            }

            break;
        }

        ret = parse_maps_entry(maps_entry, &start_addr, &end_addr);
        if (ret == ERROR) {
            break;
        }

        ret = print_pagemap_entries(pagemap_fd, start_addr, end_addr);
        if (ret == ERROR) {
            break;
        }
    }

    fclose(maps_fp);
    close(pagemap_fd);

    return ret;
}

int extract_number(const char *line, uint64_t *value, char **end_ptr) {
    errno = SUCCESS;

    *value = strtoull(line, end_ptr, 16);
    if (errno != SUCCESS) {
        perror("strtoull");
        return ERROR;
    }
    if (*end_ptr == line) {
        fprintf(stderr, "strtoull : No digits were found\n");
        return ERROR;
    }

    return SUCCESS;
}

int parse_maps_entry(const char *maps_entry, uint64_t *start_addr, uint64_t *end_addr) {
    int ret = SUCCESS;
    char *end_ptr = NULL;

    ret = extract_number(maps_entry, start_addr, &end_ptr);
    if (ret == ERROR) {
        return ERROR;
    }

    ret = extract_number(end_ptr + 1, end_addr, &end_ptr);

    return ret;
}

int print_pagemap_entries(int pagemap_fd, uint64_t start_addr, uint64_t end_addr) {
    for (uint64_t addr = start_addr; addr < end_addr; addr += PAGE_SIZE) {
        uint64_t page_num = addr / PAGE_SIZE;
        uint64_t pagemap_entry;
        ssize_t ret;

        ret = pread(pagemap_fd, &pagemap_entry, sizeof(pagemap_entry), page_num * sizeof(pagemap_entry));
        if (ret == ERROR) {
            perror("pread");
            return ERROR;
        }

        if (pagemap_entry & PAGE_PRESENT) {
            printf("%llx -> %llx\n", addr, pagemap_entry & PFN_MASK);
        }
    }

    return SUCCESS;
}
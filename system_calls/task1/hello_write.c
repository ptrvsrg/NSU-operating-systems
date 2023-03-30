#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    char buf[] = "Hello, World!\n";
    ssize_t ret;
    ssize_t written = 0;
    ssize_t len = sizeof(buf);

    while (written < len)
    {
        ret = write(STDOUT_FILENO, buf + written, len - written);

        if (ret == -1)
        {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        written += ret;
    }

    return EXIT_SUCCESS;
}

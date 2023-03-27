#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

ssize_t my_write(unsigned int fd, const char* buf, size_t len);

int main(int argc, char** argv)
{
    char buf[] = "Hello, World!\n";
    ssize_t ret;
    ssize_t written = 0;
    ssize_t len = sizeof(buf);

    while (written < len)
    {
        ret = my_write(STDOUT_FILENO, buf + written, len - written);

        if (ret == -1)
            return EXIT_FAILURE;

        written += ret;
    }

    return EXIT_SUCCESS;
}

ssize_t my_write(unsigned int fd, const char* buf, size_t len)
{
    return syscall(SYS_write, fd, buf, len);
}

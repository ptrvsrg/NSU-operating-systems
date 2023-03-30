#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>

ssize_t my_write(int fd, const void *buf, size_t size);

int main(int argc, char** argv)
{
    char buf[] = "Hello, World!\n";
    ssize_t ret;
    ssize_t written = 0;
    ssize_t len = sizeof(buf);
	
	while (written < len)
    {
        ret = my_write(54, buf + written, len - written);

        if (ret == -1)
        {
            fprintf(stderr, "Error: %s\n", strerror(errno));
            return EXIT_FAILURE;
        }

        written += ret;
    }

	return 0;
}

ssize_t my_write(int fd, const void *buf, size_t size)
{
    ssize_t ret;

	// +---------+------+------+------+
	// | syscall | arg0 | arg1 | arg2 |
	// +---------+------+------+------+
	// |   %rax  | %rdi | %rsi | %rdx |
	// +---------+------+------+------+
	asm volatile
	(
	    "syscall"
	    : "=a" (ret) 									// output
	    : "a"(SYS_write), "D"(fd), "S"(buf), "d"(size)	// input
	    : "memory"						// clobbers
	);

    return ret;
}
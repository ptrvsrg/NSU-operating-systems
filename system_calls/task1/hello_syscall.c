#define _GNU_SOURCE
#include <stdio.h>			// perror
#include <sys/syscall.h>	// SYS_write
#include <unistd.h>			// syscall

ssize_t my_write(unsigned int fd, const char* buf, size_t len);

int main(int argc, char** argv)
{
	ssize_t ret = my_write(1347, "Hello, World!\n", 15);
	
	if (ret < 0)
	{
		perror("Error");
		return -1;
	}

	return 0;
}

ssize_t my_write(unsigned int fd, const char* buf, size_t len) 
{
    return syscall(SYS_write, fd, buf, len);
}

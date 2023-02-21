#define _GNU_SOURCE
#include <stdio.h>			// perror, fprintf
#include <sys/syscall.h>	// SYS_write
#include <unistd.h>			// syscall

int main(int argc, char** argv)
{
	long ret = syscall(SYS_write, 1, "Hello, World!\n", 15);
	
	if (ret == -1)
	{
		perror("Error");
		return -1;
	}
	
	if (ret != 15)
	{
		fprintf(stderr, "Buffer is not fully written\n");
		return -1;
	}

	return 0;
}

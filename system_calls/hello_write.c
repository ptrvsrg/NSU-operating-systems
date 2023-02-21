#include <stdio.h>	// perror, fprintf
#include <unistd.h> // write

int main(int argc, char** argv)
{
	ssize_t ret;
	
	ret = write(1, "Hello, World!\n", 15);
	
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

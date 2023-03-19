#include <unistd.h>

int main(int argc, char** argv)
{
	ssize_t ret;
	char buf[] = "Hello, World!\n";
	
	ret = write(STDOUT_FILENO, buf, sizeof(buf));
	
	if (ret != sizeof(buf))
		return -1;

	return 0;
}

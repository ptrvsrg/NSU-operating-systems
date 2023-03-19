#include <sys/syscall.h>
#include <unistd.h>

ssize_t my_write(unsigned int fd, const char* buf, size_t len);

int main(int argc, char** argv)
{
	ssize_t ret;
	char buf[] = "Hello, World!\n";

	ret = my_write(STDOUT_FILENO, buf, sizeof(buf));
	
	if (ret != sizeof(buf))
		return -1;

	return 0;
}

ssize_t my_write(unsigned int fd, const char* buf, size_t len) 
{
    return syscall(SYS_write, fd, buf, len);
}

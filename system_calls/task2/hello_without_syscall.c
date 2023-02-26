#include <stdio.h>
#include <sys/syscall.h>

ssize_t my_write(int fd, const void *buf, size_t size)
{
    ssize_t ret;

	// +---------+------+------+------+------+------+------+
	// | syscall | arg0 | arg1 | arg2 | arg3 | arg4 | arg5 |
	// +---------+------+------+------+------+------+------+
	// |   %rax  | %rdi | %rsi | %rdx | %r10 | %r8  | %r9  |
	// +---------+------+------+------+------+------+------+
	// The syscall instruction itself destroys RCX and R11 
	// (to save user-space RIP and RFLAGS without needing microcode to set up the kernel stack)
    asm volatile
    (
        "syscall"
        : "=a" (ret) 									// output
        : "a"(SYS_write), "D"(fd), "S"(buf), "d"(size)	// input
        : "rcx", "r11", "memory"						// clobbers
    );

    return ret;
}

int main(int argc, char** argv)
{
    ssize_t ret;
	
	ret = my_write(1, "Hello, World!\n", 15);
	
	if (ret < 0)
	{
		perror("Error");
		return -1;
	}

	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#define CHILD_PROCESS 0
#define SUCCESS 0
#define ERROR (-1)

int execute_child_program(const char *child_program_name, char *const *child_argv);
int print_child_syscalls(pid_t child_pid);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s PROG [ARGS]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int ret = SUCCESS;
    pid_t child_pid;
    char *child_program_name = argv[1];
    char **child_argv = argv + 1;

    child_pid = fork();
    if (child_pid == ERROR) {
        perror("fork");
        return EXIT_FAILURE;
    }

    if (child_pid == CHILD_PROCESS)
        ret = execute_child_program(child_program_name, child_argv);
    else
        ret = print_child_syscalls(child_pid);

    if (ret == ERROR)
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}

int execute_child_program(const char *child_program_name, char *const *child_argv) {
    int ret = SUCCESS;

    ret = ptrace(PT_TRACE_ME, 0, NULL, NULL);
    if (ret == ERROR) {
        perror("ptrace : PT_TRACE_ME");
        return ERROR;
    }

    ret = execvp(child_program_name, child_argv);
    if (ret == ERROR) {
        perror("execvp");
        return ERROR;
    }

    return SUCCESS;
}

int print_child_syscalls(pid_t child_pid) {
    int status = 0;
    int ret = SUCCESS;
    struct user_regs_struct regs = {};

    ret = waitpid(child_pid, &status, WSTOPPED);
    if (ret == ERROR) {
        perror("waitpid");
        return ERROR;
    }

    while (WIFSTOPPED(status)) {
        ret = ptrace(PT_GETREGS, child_pid, NULL, &regs);
        if (ret == ERROR) {
            perror("ptrace : PT_GETREGS");
            return ERROR;
        }

        printf("syscall: %llu\n", regs.orig_rax);

        ret = ptrace(PT_SYSCALL, child_pid, NULL, NULL);
        if (ret == ERROR) {
            perror("ptrace : PT_SYSCALL");
            return ERROR;
        }

        ret = waitpid(child_pid, &status, WSTOPPED);
        if (ret == ERROR) {
            perror("waitpid");
            return ERROR;
        }
    }

    return SUCCESS;
}
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE 4096
#define STACK_SIZE PAGE * 8

typedef void*(*kernel_thread_routine_t)(void*);

struct kernel_thread_t {
    int                      tid;
    void*                    stack;
    kernel_thread_routine_t  routine;
    void*                    arg;
    void*                    ret;
    int                      exited;
};

void print_kernel_thread(struct kernel_thread_t* kernel_thread) {
    printf("struct kernel_thread_t {\n\taddr: %p,\n\ttid: %d,\n\tstack: %p,\n\troutine: %p,\n\targ: %p,\n\tret: %p,\n\texited: %d\n}\n", 
        kernel_thread, kernel_thread->tid, kernel_thread->stack, kernel_thread->routine, kernel_thread->arg, kernel_thread->ret, kernel_thread->exited);
}

int kernel_thread_startup(void* arg) {
    struct kernel_thread_t* kernel_thread = (struct kernel_thread_t*)arg;

    printf("kernel_thread_startup [%d]: ", kernel_thread->tid);
    print_kernel_thread(kernel_thread);
    
    kernel_thread->ret = kernel_thread->routine(kernel_thread->arg);
    kernel_thread->exited = 1;
    
    printf("kernel_thread_startup [%d]: ", kernel_thread->tid);
    print_kernel_thread(kernel_thread);
    
    return 0;
}

int kernel_thread_create(struct kernel_thread_t* kernel_thread, kernel_thread_routine_t kernel_thread_routine, void* arg) {
    static int kernel_thread_num = 0;

    kernel_thread->stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (kernel_thread->stack == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    kernel_thread->tid = kernel_thread_num++;
    kernel_thread->routine = kernel_thread_routine;
    kernel_thread->arg = arg;
    kernel_thread->exited = 0;

    struct kernel_thread_t* new_kernel_thread = kernel_thread->stack + STACK_SIZE - sizeof(struct kernel_thread_t);
    *new_kernel_thread = *kernel_thread;

    int child_pid = clone(kernel_thread_startup, kernel_thread->stack + STACK_SIZE - sizeof(struct kernel_thread_t), CLONE_VM | CLONE_SIGHAND | CLONE_THREAD, (void*) new_kernel_thread);
    if (child_pid == -1) {
        perror("clone");
        return -1;
    }

    return 0;
}

int kernel_thread_join(struct kernel_thread_t *kernel_thread, void** ret) {
    struct kernel_thread_t *new_kernel_thread = (struct kernel_thread_t *)(kernel_thread->stack + STACK_SIZE - sizeof(struct kernel_thread_t));
    while (!new_kernel_thread->exited) {
        usleep(100000);
        printf("kernel_thread_join[%d]: wait\n", new_kernel_thread->tid);
    }

    *kernel_thread = *new_kernel_thread;
    *ret = kernel_thread->ret;
    int err = munmap(kernel_thread->stack, STACK_SIZE);
    if (err == -1) {
        perror("munmap");
        return -1;
    }

    return 0;
}

void* example_routine(void* arg) {
    int a = 2;
    int b = *(int*)arg;

    int* sum = (int*)malloc(sizeof(int));
    *sum = a + b;
    sleep(2);

    return (void*)sum;
}

int main() {
    struct kernel_thread_t kernel_thread1;
    struct kernel_thread_t kernel_thread2;

    int arg1 = 3;
    int err = kernel_thread_create(&kernel_thread1, example_routine, &arg1);
    if (err == -1) {
        perror("kernel_thread_create");
        return -1;
    }
    printf("main: create thread %d\n", kernel_thread1.tid);

    int arg2 = 4;
    err = kernel_thread_create(&kernel_thread2, example_routine, &arg2);
    if (err == -1) {
        perror("kernel_thread_create");
        return -1;
    }
    printf("main: create thread %d\n", kernel_thread2.tid);
    
    void* ret;
    err = kernel_thread_join(&kernel_thread1, &ret);
    if (err == -1) {
        perror("kernel_thread_join");
        return -1;
    }
    printf("main: Sum1: %d\n", *(int*)ret);
    free(ret);

    err = kernel_thread_join(&kernel_thread2, &ret);
    if (err == -1) {
        perror("kernel_thread_join");
        return -1;
    }
    printf("main: Sum2: %d\n", *(int*)ret);
    free(ret);

    return 0;
}
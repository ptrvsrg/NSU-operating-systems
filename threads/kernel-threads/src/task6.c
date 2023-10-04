#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#define PAGE 4096
#define STACK_SIZE PAGE * 8

typedef int kernel_thread_id_t;
typedef void*(*kernel_thread_routine_t)(void*);

struct kernel_thread_t {
    kernel_thread_id_t       id;
    kernel_thread_routine_t  routine;
    void*               arg;
    void*               ret;
};

void print_kernel_thread(struct kernel_thread_t* kernel_thread) {
    printf("struct kernel_thread_t {\n\taddr: %p,\n\tid: %d,\n\troutine: %p,\n\targ: %p,\n\tret: %p\n}\n", 
        kernel_thread, kernel_thread->id, kernel_thread->routine, kernel_thread->arg, kernel_thread->ret);
}

int kernel_thread_startup(void* arg) {
    struct kernel_thread_t* kernel_thread = (struct kernel_thread_t*)arg;

    printf("kernel_thread_startup [%d]: ", kernel_thread->id);
    print_kernel_thread(kernel_thread);
    
    kernel_thread->ret = kernel_thread->routine(kernel_thread->arg);
    
    printf("kernel_thread_startup [%d]: ", kernel_thread->id);
    print_kernel_thread(kernel_thread);
    return 0;
}

int kernel_thread_create(kernel_thread_id_t* kernel_thread_id, kernel_thread_routine_t kernel_thread_routine, void* arg) {
    static int kernel_thread_num = 0;

    void* stack = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (stack == MAP_FAILED) {
        perror("mmap");
        return -1;
    }

    struct kernel_thread_t* kernel_thread = (struct kernel_thread_t*) (stack + STACK_SIZE - sizeof(struct kernel_thread_t));
    kernel_thread->id = *kernel_thread_id = kernel_thread_num++;
    kernel_thread->routine = kernel_thread_routine;
    kernel_thread->arg = arg;

    int child_ret = clone(kernel_thread_startup, stack + STACK_SIZE - sizeof(struct kernel_thread_t), 
        CLONE_VM | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD, (void*) kernel_thread);
    if (child_ret == -1) {
        perror("clone");
        return -1;
    }

    return 0;
}

void* example_routine(void* arg) {
    int a = 2;
    int b = 3;
    printf("Sum: %d\n", a + b);
    return NULL;
}

int main() {
    kernel_thread_id_t tid;
    int ret = kernel_thread_create(&tid, example_routine, NULL);
    if (ret == -1) {
        perror("kernel_thread_create");
        return -1;
    }
    printf("main: create thread %d\n", tid);

    sleep(3);

    ret = kernel_thread_create(&tid, example_routine, NULL);
    if (ret == -1) {
        perror("kernel_thread_create");
        return -1;
    }
    printf("main: create thread %d\n", tid);

    sleep(3);

    return 0;
}
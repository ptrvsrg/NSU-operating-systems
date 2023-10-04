#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

struct test_t {
    int num;
    char *str;
};

void *thread_routine(void *arg)
{
    struct test_t *test_arg = (struct test_t *) arg;

    printf("thread_routine [%d %d %d %ld]: Data: [num=%d; str=%s]\n", 
        getpid(), getppid(), gettid(), pthread_self(), test_arg->num, test_arg->str);

    return NULL;
}

int main()
{
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    struct test_t *test_arg = malloc(sizeof(struct test_t));
    
    test_arg->num = 10;

    test_arg->str = malloc(6);
    strcpy(test_arg->str, "Hello");

    pthread_t tid;
    pthread_create(&tid, NULL, thread_routine, test_arg);
    printf("main [%d %d %d]: Create thread %ld\n", getpid(), getppid(), gettid(), tid);

    pthread_join(tid, NULL);
    printf("main [%d %d %d]: Join thread_routine\n", getpid(), getppid(), gettid());
    
    free(test_arg->str);
    free(test_arg);
    
    return 0;
}
#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *thread_routine(void *arg)
{
    printf("int_routine [%d %d %d %ld]: Working\n", getpid(), getppid(), gettid(), pthread_self());
    // pthread_detach(pthread_self());
    return NULL;
}

int main()
{
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

    pthread_t tid;
    while (true)
    {
        pthread_create(&tid, NULL, thread_routine, NULL);
    }

    pthread_attr_destroy(&attr);
    return 0;
}
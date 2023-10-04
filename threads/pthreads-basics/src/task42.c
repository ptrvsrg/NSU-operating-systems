#define _GNU_SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *counter_routine(void *arg)
{
    int counter = 0;

    while (true)
    {
        // pthread_testcancel();
        counter++;
    }

    return NULL;
}

int main()
{
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    pthread_t tid;
    pthread_create(&tid, NULL, counter_routine, NULL);
    printf("main [%d %d %d]: Create thread %ld\n", getpid(), getppid(), gettid(), tid);

    sleep(5);

    pthread_cancel(tid);
    printf("main [%d %d %d]: Cancel counter_routine\n", getpid(), getppid(), gettid());

    pthread_join(tid, NULL);
    printf("main [%d %d %d]: Join counter_routine\n", getpid(), getppid(), gettid());

    return 0;
}
#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int global = 0;

void *thread_routine(void *arg)
{
    int local = 0;
    static int local_static = 0;
    const int local_const = 0;

    if (pthread_self() % 5 == 0)
    {
        local = 1;
        local_static = 1;
        global = 1;
    }

    sleep(1);
    printf("thread [%d %d %d %ld]: local[%d %p], local_static[%d %p], local_const[%d %p], global[%d %p]\n",
           getpid(), getppid(), gettid(), pthread_self(), local, &local, local_static, &local_static, local_const, &local_const, global, &global);

    return NULL;
}

int main()
{
    pthread_t tids[5];
    int err;

    printf("main [%d %d %d]: Hello from main!\n", 
           getpid(), getppid(), gettid());

    // Создать потоки
    for (int i = 0; i < 5; i++)
    {
        err = pthread_create(tids + i, NULL, thread_routine, NULL);
        if (err)
        {
            printf("main [%d %d %d]: pthread_create() failed: %s\n", 
                   getpid(), getppid(), gettid(), strerror(err));
            return -1;
        }
        printf("main [%d %d %d]: create thread %ld\n", 
               getpid(), getppid(), gettid(), tids[i]);
    }

    // Дождаться выполнения потоков
    for (int i = 0; i < 5; i++)
    {
        pthread_join(tids[i], NULL);
    }

    return 0;
}
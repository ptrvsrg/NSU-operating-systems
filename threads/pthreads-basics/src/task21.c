#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *int_routine(void *arg)
{
    // Вариант 1
    // int res = 42;
    // printf("int_routine [%d %d %d %ld]: Return 42\n", getpid(), getppid(), gettid(), pthread_self());
    // return (void *) res;

    int *res = malloc(sizeof(int));
    *res = 42;
    printf("int_routine [%d %d %d %ld]: Return %d\n", getpid(), getppid(), gettid(), pthread_self(), *res);
    return res;
}

int main()
{
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    // Создать поток для возврата числа
    pthread_t tid;
    pthread_create(&tid, NULL, int_routine, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid);

    // Дождаться завершения потока, возвращающего число
    void* res;
    pthread_join(tid, &res);
    printf("main [%d %d %d]: int_routine return %d\n", getpid(), getppid(), gettid(), *(int*)res);
    free(res);
    
    return 0;
}
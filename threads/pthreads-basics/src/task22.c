#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

void *string_routine(void *arg)
{
    char *str = malloc(14);
    strcpy(str, "Hello, World!");
    
    printf("string_routine [%d %d %d %ld]: Return %s\n", getpid(), getppid(), gettid(), pthread_self(), str);
    
    return str;
}

int main()
{
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    // Создать поток для возврата строки
    pthread_t tid;
    pthread_create(&tid, NULL, string_routine, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid);

    // Дождаться завершения потока, возвращающего строку
    void* res;
    pthread_join(tid, &res);
    printf("main [%d %d %d]: string_routine return %s\n", getpid(), getppid(), gettid(), (char*)res);
    free(res);

    return 0;
}
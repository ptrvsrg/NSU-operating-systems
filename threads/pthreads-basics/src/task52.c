#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler_sigint_1 (int signo) {
    printf("handler_sigint_1 [%d %d %d]: Received SIGINT\n", 
        getpid(), getppid(), gettid());
}

void handler_sigint_2 (int signo) {
    printf("handler_sigint_2 [%d %d %d]: Received SIGINT\n", 
        getpid(), getppid(), gettid());
}

void *handling_sigint_1_routine(void *arg) {
    struct sigaction sa;
    sa.sa_handler = handler_sigint_1;
    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGINT);
    sa.sa_mask = set; 
    sigaction(SIGINT, &sa, NULL);
    
    printf("handling_sigint_1_routine [%d %d %d]: Set up SIGINT handler\n", 
        getpid(), getppid(), gettid());

    printf("handling_sigint_1_routine [%d %d %d]: Raise SIGINT\n", 
        getpid(), getppid(), gettid());
    pthread_kill(pthread_self(), SIGINT);

    sleep(30);

    return NULL;
}

void *handling_sigint_2_routine(void *arg) {
    struct sigaction sa;
    sa.sa_handler = handler_sigint_2;
    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGINT);
    sa.sa_mask = set; 
    sigaction(SIGINT, &sa, NULL);
    
    printf("handling_sigint_2_routine [%d %d %d]: Set up SIGINT handler\n", 
        getpid(), getppid(), gettid());

    printf("handling_sigint_2_routine [%d %d %d]: Raise SIGINT\n", 
        getpid(), getppid(), gettid());
    pthread_kill(pthread_self(), SIGINT);

    sleep(30);

    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
    pthread_create(&tid1, NULL, handling_sigint_1_routine, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid1);

    pthread_create(&tid2, NULL, handling_sigint_2_routine, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid2);

    pthread_join(tid1, NULL);
    printf("main [%d %d %d]: Join handling_sigint_1_routine\n", getpid(), getppid(), gettid());

    pthread_join(tid2, NULL);
    printf("main [%d %d %d]: Join handling_sigint_2_routine\n", getpid(), getppid(), gettid());

    return 0;
}

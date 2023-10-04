#define _GNU_SOURCE
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handler_sigquit (int signo) {
    printf("handler_sigquit [%d %d %d]: Received SIGQUIT\n", 
        getpid(), getppid(), gettid());
}

void *handling_sigquit_routine(void *arg) {
    struct sigaction sa;
    sa.sa_handler = handler_sigquit;
    sigset_t set; 
    sigemptyset(&set);                                                             
    sigaddset(&set, SIGQUIT);
    sa.sa_mask = set; 
    sigaction(SIGQUIT, &sa, NULL);
    
    printf("handling_sigquit_routine [%d %d %d]: Set up SIGQUIT handler\n", 
        getpid(), getppid(), gettid());

    sleep(30);

    return NULL;
}

void *no_handling_sigquit_routine(void *arg) {
    printf("no_handling_sigquit_routine [%d %d %d]: Start\n", 
        getpid(), getppid(), gettid());

    sleep(30);

    return NULL;
}

int main() {
    pthread_t tid1, tid2;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
    pthread_create(&tid1, NULL, handling_sigquit_routine, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid1);

    pthread_create(&tid2, NULL, no_handling_sigquit_routine, NULL);
    printf("main [%d %d %d]: create thread %ld\n", getpid(), getppid(), gettid(), tid2);

    pthread_join(tid1, NULL);
    printf("main [%d %d %d]: Join handling_sigquit_routine\n", getpid(), getppid(), gettid());

    pthread_join(tid2, NULL);
    printf("main [%d %d %d]: Join no_handling_sigquit_routine\n", getpid(), getppid(), gettid());

    return 0;
}

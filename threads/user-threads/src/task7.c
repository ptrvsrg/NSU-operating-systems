#define GNU_SOURCE
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>

#define PAGE 4096
#define STACK_SIZE (PAGE * 8)
#define MAX_USER_THREADS 10

struct user_thread_t {
    int tid;
    ucontext_t ucontext;
    void *(*routine)(void *);
    void *arg;
};

struct user_thread_t user_threads[MAX_USER_THREADS];
int user_thread_count;
int current_thread_num;

void user_thread_startup(struct user_thread_t* user_thread) {
    user_thread->routine(user_thread->arg);
    free(user_thread->ucontext.uc_stack.ss_sp);
}

int user_thread_create(struct user_thread_t *user_thread,  void *(*routine)(void *), void *arg) {
    static int user_thread_tid = 0;

    if (user_thread_count >= MAX_USER_THREADS) {
        fprintf(stderr, "user_thread_create: Too many threads\n");
        return -1;
    }

    void* stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        return -1;
    }

    struct user_thread_t* new_user_thread = (struct user_thread_t*) (stack + STACK_SIZE - sizeof(struct user_thread_t));
    new_user_thread->tid = user_thread_tid++;
    new_user_thread->routine = routine;
    new_user_thread->arg = arg;

    int err = getcontext(&new_user_thread->ucontext);
    if (err == -1) {
        perror("getcontext");
        return -1;
    }
    new_user_thread->ucontext.uc_stack.ss_sp = stack;
    new_user_thread->ucontext.uc_stack.ss_size = STACK_SIZE - sizeof(struct user_thread_t);
    makecontext(&new_user_thread->ucontext, (void (*)()) user_thread_startup, 1, new_user_thread);

    user_threads[user_thread_count] = *new_user_thread;
    user_thread_count++;

    *user_thread = *new_user_thread;
    return 0;
}

void switch_user_thread() {
    ucontext_t* curr_cxt = &user_threads[current_thread_num].ucontext;
    int curr_tid = user_threads[current_thread_num].tid;

    current_thread_num = (current_thread_num + 1) % user_thread_count;
    ucontext_t* next_cxt = &user_threads[current_thread_num].ucontext;
    int next_tid = user_threads[current_thread_num].tid;

    alarm(1);
    printf("%s: Switch user thread: From %d to %d\n", __func__, curr_tid, next_tid);
    int err = swapcontext(curr_cxt, next_cxt);
    if (err == -1) {
        perror("swapcontext");
        exit(1);
    }
}

int start_user_thread_dispatcher() {
    if (user_thread_count == 0) {
        return 0;
    }

    struct sigaction sa;
    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, SIGALRM);
    sa.sa_mask = set;
    sa.sa_handler = switch_user_thread;

    int err = sigaction(SIGALRM, &sa, NULL);
    if (err == -1) {
        perror("sigaction");
        return -1;
    }

    printf("%s: Start scheduler\n", __func__);
    alarm(1);

    err = setcontext(&user_threads[0].ucontext);
    if (err == -1) {
        perror("setcontext");
        return -1;
    }

    return 0;
}

void* routine1(void* arg) {
    int counter = 0;
    while (1) {
        printf("%s: %s: Counter %d\n", __func__, (char*)arg, counter++);
        usleep(200000);
    }
}

void* routine2(void* arg) {
    int counter = 0;
    while (1) {
        printf("%s: %s: Counter %d\n", __func__, (char*)arg, counter++);
        usleep(300000);
    }
}

void* routine3(void* arg) {
    int counter = 0;
    while (1) {
        printf("%s: %s: Counter %d\n", __func__, (char*)arg, counter++);
        usleep(400000);
    }
}

int main() {
    printf("%s: pid %d\n", __func__, getpid());

    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    struct user_thread_t user_thread1, user_thread2, user_thread3;

    int err = user_thread_create(&user_thread1, routine1, "Thread 0");
    if (err == -1) {
        perror("user_thread_create");
        return EXIT_FAILURE;
    }
    printf("%s: Create thread %d\n", __func__, user_thread1.tid);

    err = user_thread_create(&user_thread2, routine2, "Thread 1");
    if (err == -1) {
        perror("user_thread_create");
        return EXIT_FAILURE;
    }
    printf("%s: Create thread %d\n", __func__, user_thread2.tid);

    err = user_thread_create(&user_thread3, routine3, "Thread 2");
    if (err == -1) {
        perror("user_thread_create");
        return EXIT_FAILURE;
    }
    printf("%s: Create thread %d\n", __func__, user_thread3.tid);

    err = start_user_thread_dispatcher();
    if (err == -1) {
        perror("start_user_thread_dispatcher");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
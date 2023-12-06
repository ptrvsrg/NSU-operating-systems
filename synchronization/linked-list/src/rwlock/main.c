#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_STRING_LENGTH 100
#define STORAGE_SIZE 1000

typedef struct _node_t {
    char val[MAX_STRING_LENGTH];
    struct _node_t *next;
    pthread_rwlock_t sync;
} node_t;

typedef struct _linked_list_t {
    node_t *first;
} linked_list_t;

node_t *create_node(char *val) {
    node_t *node = malloc(sizeof(node_t));

    strncpy(node->val, val, MAX_STRING_LENGTH);
    node->next = NULL;
    pthread_rwlock_init(&node->sync, NULL);

    return node;
}

void linked_list_destroy(linked_list_t *ll) {
    node_t *cur = ll->first;
    while (cur->next != NULL) {
        node_t *tmp = cur->next;
        free(cur);
        cur = tmp;
    }
    free(cur);
}

int asc_count = 0;
int asc_iter = 0;

void *asc_routine(void *args) {
    linked_list_t *ll = (linked_list_t *) args;

    while (true) {
        pthread_rwlock_rdlock(&ll->first->sync);
        node_t *prev = ll->first, *cur;
        while (prev->next != NULL) {
            cur = prev->next;
            int size = strlen(prev->val);
            pthread_rwlock_rdlock(&cur->sync);
            pthread_rwlock_unlock(&prev->sync);
            if (size < strlen(cur->val)) {
                asc_count++;
            }
            prev = cur;
        }
        pthread_rwlock_unlock(&prev->sync);
        ++asc_iter;
    }
}

int desc_count = 0;
int desc_iter = 0;

void *desc_routine(void *args) {
    linked_list_t *ll = (linked_list_t *) args;

    while (true) {
        pthread_rwlock_rdlock(&ll->first->sync);
        node_t *prev = ll->first, *cur;
        while (prev->next != NULL) {
            cur = prev->next;
            int size = strlen(prev->val);
            pthread_rwlock_rdlock(&cur->sync);
            pthread_rwlock_unlock(&prev->sync);
            if (size > strlen(cur->val)) {
                desc_count++;
            }
            prev = cur;
        }
        pthread_rwlock_unlock(&prev->sync);
        ++desc_iter;
    }
}

int eq_count = 0;
int eq_iter = 0;

void *eq_routine(void *args) {
    linked_list_t *ll = (linked_list_t *) args;

    while (true) {
        pthread_rwlock_rdlock(&ll->first->sync);
        node_t *prev = ll->first, *cur;
        while (prev->next != NULL) {
            cur = prev->next;
            int size = strlen(prev->val);
            pthread_rwlock_rdlock(&cur->sync);
            pthread_rwlock_unlock(&prev->sync);
            if (size == strlen(cur->val)) {
                eq_count++;
            }
            prev = cur;
        }
        pthread_rwlock_unlock(&prev->sync);
        ++eq_iter;
    }
}

int swap_count = 0;
int swap_iter = 0;

void *swap_routine(void *args) {
    linked_list_t *ll = (linked_list_t *) args;

    while (true) {
        pthread_rwlock_rdlock(&ll->first->sync);
        node_t *prev = ll->first, *cur, *next;
        while (prev->next != NULL) {
            if (rand() % 100 != 0) {
                cur = prev->next;
                pthread_rwlock_rdlock(&cur->sync);
                pthread_rwlock_unlock(&prev->sync);
                prev = cur;
                continue;
            }
            cur = prev->next;
            pthread_rwlock_wrlock(&cur->sync);
            next = cur->next;
            if (next == NULL) {
                pthread_rwlock_unlock(&cur->sync);
                break;
            }
            pthread_rwlock_wrlock(&next->sync);
            prev->next = next;
            pthread_rwlock_unlock(&prev->sync);
            cur->next = next->next;
            pthread_rwlock_unlock(&cur->sync);
            next->next = cur;
            swap_count++;
            prev = next;
            pthread_rwlock_unlock(&prev->sync);
            pthread_rwlock_rdlock(&prev->sync);
        }
        pthread_rwlock_unlock(&prev->sync);
        ++swap_iter;
    }
}

void *print_routine(void *args) {
    while (true) {
        sleep(1);
        printf("asc: %d/%d,\t\tdesc: %d/%d,\t\teq: %d/%d,\t\tswap: %d/%d\n", asc_count, asc_iter, desc_count, desc_iter,
               eq_count, eq_iter, swap_count, swap_iter);
    }
}

int main() {
    linked_list_t *ll = malloc(sizeof(linked_list_t));

    srand(time(NULL));
    char val[MAX_STRING_LENGTH];
    memset(val, 0, MAX_STRING_LENGTH);
    memset(val, '0', rand() % MAX_STRING_LENGTH);
    ll->first = create_node(val);

    node_t *last = ll->first;
    for (int i = 1; i < STORAGE_SIZE; ++i) {
        memset(val, 0, MAX_STRING_LENGTH);
        memset(val, '0', rand() % MAX_STRING_LENGTH);
        node_t *newNode = create_node(val);
        last->next = newNode;
        last = newNode;
    }

    pthread_t tid[6];

    int err = pthread_create(&tid[0], NULL, asc_routine, ll);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[1], NULL, desc_routine, ll);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[2], NULL, eq_routine, ll);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    err = pthread_create(&tid[3], NULL, print_routine, NULL);
    if (err) {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }

    for (int i = 0; i < 3; ++i) {
        err = pthread_create(&tid[4 + i], NULL, swap_routine, ll);
        if (err) {
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }

    sleep(10);

    for (int i = 0; i < 6; ++i) {
        pthread_cancel(tid[i]);
    }

    linked_list_destroy(ll);
    free(ll);
}

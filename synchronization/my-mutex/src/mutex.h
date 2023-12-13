#ifndef NSU_OPERATING_SYSTEMS_MUTEX_H
#define NSU_OPERATING_SYSTEMS_MUTEX_H

#include <stdatomic.h>

typedef struct _mutex {
    int lock;
    int tid;
} mutex_t;

void mutex_init(mutex_t *m);
void mutex_lock(mutex_t *m);
void mutex_unlock(mutex_t *m);

#endif //NSU_OPERATING_SYSTEMS_MUTEX_H

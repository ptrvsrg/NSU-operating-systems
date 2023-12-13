#ifndef NSU_OPERATING_SYSTEMS_SPINLOCK_H
#define NSU_OPERATING_SYSTEMS_SPINLOCK_H

#include <stdatomic.h>

typedef int spinlock_t;

void spinlock_init(spinlock_t* s);
void spinlock_lock(spinlock_t* s);
void spinlock_unlock(spinlock_t *s);

#endif //NSU_OPERATING_SYSTEMS_SPINLOCK_H

/*
 *
 * tinythreads.h
 *
 */

#ifndef _TINYTHREADS_H
#define _TINYTHREADS_H

#include <stdint.h>

struct thread_block;
typedef struct thread_block *thread;
void spawn(void (*code)(uint8_t), uint8_t arg);
void yield(void);

struct mutex_block {
    int locked;
    thread waitQ;
};
typedef struct mutex_block mutex;

#define MUTEX_INIT {0,0}
void lock(mutex *m);
void unlock(mutex *m);

#endif

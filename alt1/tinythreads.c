#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "avrinit.h"
#include "tinythreads.h"
#include "avrprint.h" // TODO rm testing

#define NULL            0
#define DISABLE()       cli()
#define ENABLE()        sei()
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf,a) *((uint16_t *)(buf)+8) = (uint16_t)(a) + STACKSIZE - 4; \
                        *((uint16_t *)(buf)+9) = (uint16_t)(a) + STACKSIZE - 4


struct thread_block {
    void (*function)(uint8_t);   // code to run
    uint8_t arg;                 // argument to the above
    thread next;                 // for use in linked lists
    jmp_buf context;             // machine state
    char stack[STACKSIZE];       // execution stack space
};

struct thread_block threads[NTHREADS];

struct thread_block initp;

thread freeQ   = threads;
thread readyQ  = NULL;
thread current = &initp;

mutex m = MUTEX_INIT;

uint8_t initialized = 0;
uint8_t joyDown = 0;
uint16_t timerInterruptCounter = 0;


static void initialize(void) {

    initializeAVR();

    int i;
    for (i=0; i<NTHREADS-1; i++)
        threads[i].next = &threads[i+1];
    threads[NTHREADS-1].next = NULL;


    initialized = 1;
}


static void enqueue(thread p, thread *queue) {
    p->next = NULL;
    if (*queue == NULL) {
        *queue = p;
    } else {
        thread q = *queue;
        while (q->next)
            q = q->next;
        q->next = p;
    }
}

static thread dequeue(thread *queue) {
    thread p = *queue;
    if (*queue) {
        *queue = (*queue)->next;
    } else {
        writeChar('9', 2);
        // Empty queue, kernel panic!!!
        while (1) ;  // not much else to do...
    }
    return p;
}

static void dispatch(thread next) {
    if (setjmp(current->context) == 0) {
        current = next;
        longjmp(next->context,1);
    }
}

void spawn(void (* function)(uint8_t), uint8_t arg) {
    thread newp;

    DISABLE();
    if (!initialized) initialize();

    newp = dequeue(&freeQ);
    newp->function = function;
    newp->arg = arg;
    newp->next = NULL;
    if (setjmp(newp->context) == 1) {
        ENABLE();
        current->function(current->arg);
        DISABLE();
        enqueue(current, &freeQ);
        dispatch(dequeue(&readyQ));
    }
    SETSTACK(&newp->context, &newp->stack);

    enqueue(newp, &readyQ);
    ENABLE();
}

void yield(void) {
    DISABLE();
    enqueue(current, &readyQ);  // Add current back to normal queue
    dispatch(dequeue(&readyQ)); // Dispatch next thread
    ENABLE();
}

void lock(mutex *m) {
    DISABLE();
    if (m->locked) {
        enqueue(current, &(m->waitQ)); // Add current to mutex queue
        dispatch(dequeue(&readyQ)); // Run something else while waiting for lock
    }
    else {
        m->locked = 1;
    }
    ENABLE();
}

void unlock(mutex *m) {
    DISABLE();
    if (m->waitQ == NULL) {
        m->locked = 0;
    }
    else {
        enqueue(current, &readyQ);  // Add current back to normal queue
        dispatch(dequeue(&(m->waitQ))); // Dispatch next from mutex queue
    }
    ENABLE();
}

uint16_t rtic() {
    DISABLE();
    uint16_t old = timerInterruptCounter;
    timerInterruptCounter = 0;
    ENABLE();
    return old;
}


/* Joy down input interrupt */
ISR(PCINT1_vect) {
    DISABLE();
    if (!joyDown) {
        joyDown = 1;
        yield();
    } else if (joyDown) {
        joyDown = 0;
        ENABLE();
    }
}

/* Timer interrupt */
ISR(TIMER1_COMPA_vect) {
    timerInterruptCounter++;
    yield();
}


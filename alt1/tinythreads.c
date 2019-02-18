#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "tinythreads.h"
#include "avrprint.h" // TODO rm testing

#define NULL            0
#define DISABLE()       cli()
#define ENABLE()        sei()
#define STACKSIZE       80
#define NTHREADS        4
#define SETSTACK(buf,a) *((uint16_t *)(buf)+8) = (uint16_t)(a) + STACKSIZE - 4; \
                        *((uint16_t *)(buf)+9) = (uint16_t)(a) + STACKSIZE - 4

#define TCYCLES         391  // 8 M / 1024 / (50 / 1000) = 390.625

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

uint8_t initialized = 0;
uint8_t joyDown = 0;

static void initInt(void) {

    // Port B pins alternate functions
    PORTB   |= (1<<PB7) // "Activate" down input from joystick (Pin Change INT15)
            | (1<<PB5); // Output Compare Match A output (Timer/counter1)

    // Port B Data Direction Register
    //DDRB = (1<<DDB5);   // Configure pin as output (Timer/counter1)

    // External Interrupt Mask Register
    //EIMSK = (1<<PCIE1); // Pin Change Interrupt Enable for PCINT15..8

    // Pin Change Mask Register for PCINT15..8
    //PCMSK1 = (1<<PCINT15); // Pin change interrupt enabled for Port B7

    // Timer/Counter1 Interrupt Mask Register
    TIMSK1 = (1<<OCIE1A); // Output Compare A Match Interrupt Enable

    // Timer/Counter1 Control Register B
    TCCR1B =  (1<<CS12) | (1<<CS10) // System clk with 1024 prescaler factor 
            | (1<<WGM12);           // Clear Timer on Compare mode for OCR1A
}


static void initialize(void) {
    int i;
    for (i=0; i<NTHREADS-1; i++)
        threads[i].next = &threads[i+1];
    threads[NTHREADS-1].next = NULL;

    initInt();

    // Set Output Compare Register A
    OCR1A = TCYCLES;

    TCNT1 = 0; // clear system clk

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

ISR(PCINT1_vect) {
    DISABLE();
    if (!joyDown) {
        joyDown = 1;
        yield();
    } else if (joyDown) {
        joyDown = 0;
    }
}

ISR(TIMER1_COMPA_vect) {
    yield();
}


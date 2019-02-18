#include "tinythreads.h"
#include "avrprint.h"       // Lab 1 stuff
#include "avrinit.h"
#include "primes.h"

#include <stdint.h>

void printAt(uint8_t num, uint8_t pos);
void computePrimes(uint8_t pos);

mutex m = MUTEX_INIT;


int main(void) {

    initCPU();
    initLCD();
    //initInt();
    //initInput();

    //TCNT1 = 0; // clear system clk
    //// Set Output Compare Register A
    //OCR1A = 391;  // 8 M / 1024 / 20


    //while (1) {
    //    writeLong((long) TCNT1);
    //}

    //spawn(computePrimes, 0);
    //computePrimes(3);

    return 0;
}


void printAt(uint8_t num, uint8_t pos) {
    uint8_t pp = pos;
    writeChar( (num % 100) / 10 + '0', pp);
    int i;
    for (i = 0; i<50000; i++);
    pp++;
    writeChar( num % 10 + '0', pp);
}


void computePrimes(uint8_t pos) {
    long n;

    for(n = 1; ; n++) {
        if (isPrime(n)) {
            printAt(n, pos);
            //yield();
        }
    }
}


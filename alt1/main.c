#include "tinythreads.h"
#include "avrprint.h"       // Lab 1 stuff
#include "primes.h"
#include "blink.h"
#include "button.h"

#include <stdint.h>

void computePrimes(uint8_t pos);


int main(void) {

    spawn(computePrimes, 0);
    spawn(button, 4);
    blink();
    //computePrimes(3);

    return 0;
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


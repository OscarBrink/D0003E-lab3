#include "tinythreads.h"
#include "avrprint.h"
#include "primes.h"
#include "blink.h"
#include "button.h"

#include <stdint.h>

void computePrimes(uint8_t pos);


int main(void) {
    spawn(blink, 0);
    button(4);
    computePrimes(0);

    return 0;
}


void computePrimes(uint8_t pos) {
    long n;

    for(n = 1; ; n++) {
        if (isPrime(n)) {
            printAt(n, pos);
        }
    }
}


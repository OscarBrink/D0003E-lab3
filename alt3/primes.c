#include "primes.h"

uint8_t primes(void) {

    long p = 2;
    while (1) {
        if (isPrime(p)) {
            writeLong(p);
        }
        p++;
    }

    return 0;
}

uint8_t isPrime(long i) {

    if (i == 2 || i == 1) {
        return 1;
    }

    for (long j = 2; j < (i >> 1); j++) {
        if (i % j == 0) {
            return 0;
        }
    }
    return 1;
}

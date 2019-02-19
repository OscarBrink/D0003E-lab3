#include "blink.h"
#include "avrprint.h"

//#define CLKPERIOD 3906 // (8 M / 1024) / 2 = 3906.25

// 8 MHz clock
// 256 Prescaling
uint16_t blink(void) {

    LCDDR0 |= 0x40; // init state

    while (1) {
        lock(&mTimer);
        LCDDR0 ^= 0x40; // Flip the bit
    }

    return 0;
}


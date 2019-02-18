#include "blink.h"
#include "avrprint.h"

#define CLKPERIOD 3906 // (8 M / 1024) / 2 = 3906.25

// 8 MHz clock
// 256 Prescaling
uint16_t blink(void) {

    uint16_t nextStateChange = CLKPERIOD;
    uint16_t timerValue = 0;

    LCDDR0 |= 0x40; // init state

    while (1) {

        while (timerValue < nextStateChange) { // Busy wait
            timerValue += rtic() * TCYCLES;
        }

        LCDDR0 ^= 0x40; // Flip the bit

        nextStateChange += CLKPERIOD;

        while (timerValue > nextStateChange) { // Busy wait if and until wraparound happens
            timerValue += rtic() * TCYCLES;
        }
    }

    return 0;
}


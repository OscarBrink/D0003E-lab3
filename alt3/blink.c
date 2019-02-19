#include "blink.h"
#include "avrprint.h"

uint8_t blinkInitialized = 0;

void blink(uint8_t dud) {

    if (!blinkInitialized) {
        LCDDR0 |= 0x40; // init state
        blinkInitialized = 1;
    } else {
        LCDDR0 ^= 0x40; // Flip the bit
    }
}


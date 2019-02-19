#include "button.h"
#include "avrprint.h"

uint8_t timesPressed = 0;
uint8_t buttonInitialized = 0;

void button(uint8_t pos) {
    if (!buttonInitialized) {
        LCDDR1 = 0x20; // Set initial state
        buttonInitialized = 1;
    } else {
        LCDDR1 ^= 0x60;
    }

    printAt(++timesPressed, pos);
}


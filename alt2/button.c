#include "button.h"
#include "avrprint.h"

uint8_t timesPressed = 0;

void button(uint8_t pos) {

    LCDDR1 = 0x20; // Set initial state
    printAt(timesPressed, pos);

    while (1) {
        lock(&mInput);
        changeState(pos);
    }

}

void changeState(uint8_t pos) {
    printAt(++timesPressed, pos);
    LCDDR1 ^= 0x60;
}


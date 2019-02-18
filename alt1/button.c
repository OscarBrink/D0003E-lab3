#include "button.h"
#include "avrprint.h"

uint8_t timesPressed = 0;

void button(uint8_t pos) {

    LCDDR1 = 0x20; // Set initial state
    printAt(timesPressed, pos);

    while (1) {
        while (PINB & (1<<PINB7)); // busy wait until input
        changeState(pos);
        while ( !(PINB & (1<<PINB7)) ); // busy wait until joystick is released
    }

}

void changeState(uint8_t pos) {
    printAt(++timesPressed, pos);
    LCDDR1 ^= 0x60;
}


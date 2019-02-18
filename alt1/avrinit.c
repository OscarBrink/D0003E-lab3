#include "avrinit.h"


/*
 * Initialize CPU
 */
void initCPU(void) {
    // Disables the CPU clock prescaler
    CLKPR = 0x80;
    CLKPR = 0x00;
}


/*
 * Initializer for the LCD display
 * See comments below for options set
 */
void initLCD(void) {

    // LCD Control Register B
    LCDCRB = (1<<LCDCS)                             // Async clock select
        | (1<<LCDMUX1) | (1<<LCDMUX0)               // 1/3 bias
        | (1<<LCDPM2) | (1<<LCDPM1) | (1<<LCDPM0);  // Port pins to use as segment drivers (25)

    // LCD Frame Rate Register
    // LCD Prescaler Select (3 bits): D = 16
    LCDFRR = (1<<LCDCD2) | (1<<LCDCD1) | (1<<LCDCD0); // LCD Clock Divide = 8

    // LCD Contrast Control
    // LCD Display Configuration (3 bits): 300 us Nominal drive time
    LCDCCR =  (1<<LCDCC3) | (1<<LCDCC2) | (1<<LCDCC1) | (1<<LCDCC0); // LCD Max voltage = 3.35 V

    // LCD Control and Status
    LCDCRA = (1<<LCDEN) // LCD Enable
        | (1<<LCDAB);   // LCD Low Power Waveform
}


/*
 * Initializes the 8 Mhz clock
 */
void initClk(void) {
    // Timer/Counter1 Control Register B
    TCCR1B = (1<<CS12); // System clk with 256 prescaler factor
}


/*
 * Initializes input
 */
void initInput(void) {
    // "Activate" down input from joystick (bit 7 in PORTB)
    PORTB |= (1<<PB7);

}


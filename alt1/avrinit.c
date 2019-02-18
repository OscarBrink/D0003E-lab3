#include "avrinit.h"
#include "avrprint.h"

void initializeAVR(void) {
    initCPU();
    initLCD();
    initInput();
    initInt();
    initClk();
}


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
    // Port B pins alternate functions
    PORTB |= (1<<PB5); // Output Compare Match A output (Timer/counter1)

    // Timer/Counter1 Control Register B
    TCCR1B =  (1<<CS12) | (1<<CS10) // System clk with 1024 prescaler factor 
            | (1<<WGM12);           // Clear Timer on Compare mode for OCR1A

    // Set Output Compare Register A
    OCR1A = TCYCLES;

    TCNT1 = 0; // clear system clk
}


/*
 * Initializes input
 */
void initInput(void) {
    // Port B pins alternate functions
    PORTB |= (1<<PB7); // "Activate" down input from joystick (Pin Change INT15)
}

void initInt(void) {

    // Timer/Counter1 Interrupt Mask Register
    TIMSK1 = (1<<OCIE1A); // Output Compare A Match Interrupt Enable

    // Port B Data Direction Register
    //DDRB = (1<<DDB5);   // Configure pin as external output (Timer/counter1)

    // External Interrupt Mask Register
    //EIMSK = (1<<PCIE1); // Pin Change Interrupt Enable for PCINT15..8

    // Pin Change Mask Register for PCINT15..8
    //PCMSK1 = (1<<PCINT15); // Pin change interrupt enabled for Port B7
}

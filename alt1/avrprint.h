#include <stdint.h>
#include <avr/io.h>

uint8_t writeChar(char ch, int pos);
uint8_t writeLong(long i);
uint8_t mapLCDSegments(uint8_t *segmentMap, char ch);

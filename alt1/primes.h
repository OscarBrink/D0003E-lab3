#include <avr/io.h>

uint8_t writeChar(char ch, int pos);
uint8_t mapLCDSegments(uint8_t *segmentMap, char ch);
uint8_t writeLong(long i);
uint8_t primes(void);
uint8_t isPrime(long i);
void clearChar(int pos);


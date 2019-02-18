#include <avr/io.h>

#define TCYCLES 391  // 8 M / 1024 / (50 / 1000) = 390.625

void initializeAVR(void);
void initCPU(void);
void initLCD(void);
void initClk(void);
void initInput(void);
void initInt(void);


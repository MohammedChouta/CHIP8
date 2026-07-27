#ifndef CHIP8
#define CHIP8

#include <stdint.h>

typedef struct {
    uint8_t display[64][32];
    uint8_t MEMORY[4096];
    uint8_t V[16];
    uint16_t I;
    uint8_t delay;
    uint8_t ST;
    uint16_t PC;
    uint8_t SP;
    uint16_t stack[16];
    uint8_t keypad[16];
} chip8;

#endif

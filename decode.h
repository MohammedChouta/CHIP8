#ifndef DECODE
#define DECODE

#include <stdint.h>
#include "chip8.h"

void decode(uint16_t instruction, chip8 *c8);
uint16_t extract(uint16_t instruction, char data[]);


#endif
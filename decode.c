#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "chip8.h"
#include "decode.h"


uint16_t extract(uint16_t instruction, char data[]){

    uint16_t extracted;

    if (strcmp(data, "nnn") == 0){
        extracted = instruction << 4;
        extracted = extracted >> 4;
    }
    else if (strcmp(data, "n") == 0){
        extracted = instruction << 12;
        extracted = extracted >> 12;        
    }
    else if(strcmp(data, "x") == 0){
        extracted = instruction << 4;
        extracted = extracted >> 12;
    }
    else if(strcmp(data, "y") == 0){
        extracted = instruction << 8;
        extracted = extracted >> 12;
    }
    else if(strcmp(data, "kk") == 0){
        extracted = instruction << 8;
        extracted = extracted >> 8;
    }

    return extracted;
}

void decode(uint16_t instruction, chip8 *c8){

    uint16_t nnn = extract(instruction, "nnn");
    uint8_t n = extract(instruction, "n");
    uint8_t x = extract(instruction, "x");
    uint8_t y = extract(instruction, "y");
    uint8_t kk = extract(instruction, "kk");


    switch(instruction >> 12){
        case 0:
            switch(nnn){
                case 0x0E0:
                    memset(c8->display, 0, sizeof(c8->display));
                    c8->PC+=2;
                break;
                case 0x0EE:
                c8->PC = c8->stack[c8->SP - 1];
                c8->SP -= 1;
                break;
                default:
                break;
            }
        break;
        case 1:
            c8->PC = nnn;
        break;
        case 2:
            c8->SP += 1;
            c8->stack[c8->SP - 1] = c8->PC + 2;
            c8->PC = nnn;
        break;
        case 3:
            if(c8->V[x] == kk)
                c8->PC += 4;
            else
                c8->PC += 2;
        break;
        case 4:
            if(c8->V[x] != kk)
                c8->PC += 4;
            else
                c8->PC += 2;
        break;
        case 5:
            if(c8->V[x] == c8->V[y])
                c8->PC += 4;
            else
                c8->PC += 2;
        break;
        case 6:
            c8->V[x] = kk;
            c8->PC+=2;
        break;
        case 7:
            c8->V[x] += kk;
            c8->PC+=2; 
        break;
        case 8:
            switch(n){
                case 0:
                    c8->V[x] = c8->V[y];
                    c8->PC+=2;
                break;
                case 1:
                    c8->V[x] |= c8->V[y];
                    c8->PC+=2; 
                break;
                case 2:
                    c8->V[x] &= c8->V[y];
                    c8->PC+=2; 
                break;
                case 3:
                    c8->V[x] ^= c8->V[y];
                    c8->PC+=2; 
                break;
                case 4:
                    if (c8->V[x] + c8->V[y] > 255)
                        c8->V[0xF] = 1;
                    else
                        c8->V[0xF] = 0;
                    c8->V[x] += c8->V[y];
                    c8->PC+=2;
                break;
                case 5:
                    if (c8->V[x] > c8->V[y])
                        c8->V[0xF] = 1;
                    else
                        c8->V[0xF] = 0;
                    c8->V[x] -= c8->V[y];
                    c8->PC+=2;
                break;
                case 6:
                    if (c8->V[x] & 0b00000001)
                        c8->V[0xF] = 1;
                    else
                        c8->V[0xF] = 0;
                    c8->V[x] /= 2;
                    c8->PC+=2;
                break;
                case 7:
                    if (c8->V[y] > c8->V[x])
                        c8->V[0xF] = 1;
                    else
                        c8->V[0xF] = 0;
                    c8->V[x] = c8->V[y] - c8->V[x];
                    c8->PC+=2;
                break;
                case 0xE:
                    if (c8->V[x] & 0b10000000)
                        c8->V[0xF] = 1;
                    else
                        c8->V[0xF] = 0;
                    c8->V[x] *= 2;
                    c8->PC+=2;
                break;
            }
        break;
        case 9:
            if(c8->V[x] != c8->V[y])
                c8->PC += 4;
            else
                c8->PC += 2;
        break;
        case 0xA:
            c8->I = nnn;
            c8->PC+=2;
        break;
        case 0xB:
            c8->PC = nnn + c8->V[0];
        break;
        case 0xC:
            c8->V[x] = (rand() % (255 + 1)) & kk;
            c8->PC+=2;
        break;
        case 0xD:
            c8->V[0xF] = 0;

            for (int row = 0; row < n; row++) {
                uint8_t sprite = c8->MEMORY[c8->I + row];
                for (int col = 0; col < 8; col++) {

                    if (sprite & (0b10000000 >> col)) {

                        int x_ = (c8->V[x] + col) % 64;
                        int y_ = (c8->V[y] + row) % 32;

                        if (c8->display[x_][y_] == 1)
                            c8->V[0xF] = 1;

                        c8->display[x_][y_] ^= 1;
                    }
                }
            }
            c8->PC+=2;
        break;
        case 0xE:
            switch(kk){
                case 0x9E:
                    if (c8->keypad[c8->V[x]])
                        c8->PC += 4;
                    else
                        c8->PC += 2;
                break;
                case 0xA1:
                    if (!c8->keypad[c8->V[x]])
                        c8->PC += 4;
                    else
                        c8->PC += 2;
                break;
            }
        break;
        case 0xF:
            switch (kk)
            {
            case 0x07:
                c8->V[x] = c8->delay;
                c8->PC+=2;
            break;
            case 0x0A:
                for(int i = 0; i < 16; i++){
                    if(c8->keypad[i] == 1){
                        c8->V[x] = i;
                        c8->PC+=2;
                    }
                }
            break;
            case 0x15:
                c8->delay = c8->V[x];
                c8->PC+=2;
            break;
            case 0x18:
                c8->ST = c8->V[x];
                c8->PC+=2;
            break;
            case 0x1E:
                c8->I += c8->V[x];
                c8->PC+=2;
            break;
            case 0x29:
                c8->I = c8->V[x] * 5;
                c8->PC+=2;
            break;
            case 0x33:
                c8->MEMORY[c8->I]     = c8->V[x] / 100;
                c8->MEMORY[c8->I + 1] = (c8->V[x] / 10) % 10;
                c8->MEMORY[c8->I + 2] = c8->V[x] % 10;
                c8->PC+=2;               
            break;
            case 0x55:
                for(int i = 0; i <= x; i++){
                    c8->MEMORY[c8->I + i] = c8->V[i];
                }
                c8->PC+=2;
            break;
            case 0x65:
                for(int i = 0; i <= x; i++){
                    c8->V[i] = c8->MEMORY[c8->I + i];
                }
                c8->PC+=2;            
            break;
            }
        break;
        default:
        break;
    }
}
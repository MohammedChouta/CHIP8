#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <time.h>
#include "chip8.h"
#include "decode.h"

int main(int argc, char** argv) {

    FILE *rom = fopen(argv[1], "rb");

    uint8_t sprite_data[] = {0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                            0x20, 0x60, 0x20, 0x20, 0x70, // 1
                            0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                            0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                            0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                            0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                            0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                            0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                            0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                            0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                            0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                            0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                            0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                            0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                            0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                            0xF0, 0x80, 0xF0, 0x80, 0x80  // F
                            };

    if(rom == NULL){
        perror("Error opening the file\n");
        return EXIT_FAILURE;
    }

    fseek(rom, 0, SEEK_END);
    size_t filesize = ftell(rom);
    rewind(rom);

    uint8_t dump[filesize];
    fread(dump, sizeof(uint8_t), filesize, rom);
    
    chip8 c8;
    memset(c8.MEMORY, 0, sizeof(c8.MEMORY));
    memset(c8.stack, 0, sizeof(c8.stack));
    memset(c8.keypad, 0, sizeof(c8.keypad));
    memset(c8.display, 0, sizeof(c8.display));

    memcpy(c8.MEMORY, sprite_data, 80);
    memcpy(c8.MEMORY + 0x200, dump, filesize);

    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED, 640, 320, 0);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    c8.PC = 0x200;
    c8.SP = 0;
    srand(time(NULL));
    int quit = 0;
    while(!quit){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            switch (e.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
                switch (e.key.keysym.sym)
                {
                case SDLK_1:
                    c8.keypad[0x1] = 1;
                    break;
                case SDLK_2:
                    c8.keypad[0x2] = 1;
                    break;
                case SDLK_3:
                    c8.keypad[0x3] = 1;
                    break;
                case SDLK_4:
                    c8.keypad[0xC] = 1;
                    break;
                case SDLK_q:
                    c8.keypad[0x4] = 1;
                    break;
                case SDLK_w:
                    c8.keypad[0x5] = 1;
                    break;
                case SDLK_e:
                    c8.keypad[0x6] = 1;
                    break;
                case SDLK_r:
                    c8.keypad[0xD] = 1;
                    break;
                case SDLK_a:
                    c8.keypad[0x7] = 1;
                    break;
                case SDLK_s:
                    c8.keypad[0x8] = 1;
                    break;
                case SDLK_d:
                    c8.keypad[0x9] = 1;
                    break;
                case SDLK_f:
                    c8.keypad[0xE] = 1;
                    break;
                case SDLK_z:
                    c8.keypad[0xA] = 1;
                    break;
                case SDLK_x:
                    c8.keypad[0x0] = 1;
                    break;
                case SDLK_c:
                    c8.keypad[0xB] = 1;
                    break;
                case SDLK_v:
                    c8.keypad[0xF] = 1;
                    break;
                default:
                    break;
                }
            break;
            case SDL_KEYUP:
                switch (e.key.keysym.sym)
                {
                case SDLK_1:
                    c8.keypad[0x1] = 0;
                    break;
                case SDLK_2:
                    c8.keypad[0x2] = 0;
                    break;
                case SDLK_3:
                    c8.keypad[0x3] = 0;
                    break;
                case SDLK_4:
                    c8.keypad[0xC] = 0;
                    break;
                case SDLK_q:
                    c8.keypad[0x4] = 0;
                    break;
                case SDLK_w:
                    c8.keypad[0x5] = 0;
                    break;
                case SDLK_e:
                    c8.keypad[0x6] = 0;
                    break;
                case SDLK_r:
                    c8.keypad[0xD] = 0;
                    break;
                case SDLK_a:
                    c8.keypad[0x7] = 0;
                    break;
                case SDLK_s:
                    c8.keypad[0x8] = 0;
                    break;
                case SDLK_d:
                    c8.keypad[0x9] = 0;
                    break;
                case SDLK_f:
                    c8.keypad[0xE] = 0;
                    break;
                case SDLK_z:
                    c8.keypad[0xA] = 0;
                    break;
                case SDLK_x:
                    c8.keypad[0x0] = 0;
                    break;
                case SDLK_c:
                    c8.keypad[0xB] = 0;
                    break;
                case SDLK_v:
                    c8.keypad[0xF] = 0;
                    break;
                default:
                    break;
                }
            break;
            default:
                break;
            }
        }

        for(int i = 0; i < 10; i++){
            uint16_t instruction = (c8.MEMORY[c8.PC] << 8) + c8.MEMORY[c8.PC + 1];
            decode(instruction, &c8);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        for(int x = 0; x < 64; x++){
            for(int y = 0; y < 32; y++){
                if(c8.display[x][y] == 1){
                    SDL_Rect r = { x * 10, y * 10, 10, 10 };
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
        SDL_RenderPresent(renderer);
    
        if (c8.delay > 0) c8.delay--;
        if (c8.ST > 0) c8.ST--;

    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    fclose(rom);
    return EXIT_SUCCESS;
}
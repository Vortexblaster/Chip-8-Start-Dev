#ifndef STD_H_
#define STD_H_
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#endif
#ifndef CORE_H_
#include "SDL3/SDL.h"
#include "SDL3/SDL_keycode.h"
#include "SDL3/SDL_pixels.h"
typedef struct Frame_s {
    SDL_Surface * frameSurface;
    SDL_Palette * surfacePalette;
    //uint8_t * * screenArray;
    uint16_t resolutionX;
    uint16_t resolutionY;
} Frame_t;
#endif
#ifndef CHIP_8_H_
#define CHIP_8_H_
#define memory_size 4096
#define address_size 16
#define register_size 16
#define pc_size 12
#define keypad_size 16
typedef struct Cpu_s Cpu_t;
typedef void (*Cycle)(Cpu_t *, Frame_t *);
typedef struct Cpu_s {
    uint8_t memory[memory_size];
    uint8_t v[register_size];
    uint16_t stack[pc_size];
    uint16_t pc;
    uint16_t I;
    uint8_t sound;
    uint8_t delay;
    Cycle cycle;
    uint8_t clockSpeedMHz;
    char * architecture;
    SDL_Keycode keypad[keypad_size*2];
    SDL_Keycode currentKey;
} Cpu_t;
void cycle(Cpu_t *, Frame_t *);
int pop(uint8_t *, uint16_t *);
void push(uint8_t *, uint16_t *, uint16_t);
Cpu_t createEmulator(char *, uint8_t, uint8_t *, uint8_t, uint8_t *, uint16_t, SDL_Keycode *);
void destroyEmulator(Cpu_t *);
#endif

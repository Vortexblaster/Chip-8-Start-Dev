#ifndef STD_H_
#define STD_H_
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#endif
#ifndef VIDEO_H_
#include "SDL3/SDL.h"
typedef struct Frame_s {
    SDL_Surface * frameSurface;
    //uint8_t * screenArray;
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
typedef struct Cpu_s Cpu_t;
typedef void (*Cycle)(Cpu_t *, Frame_t *);
typedef struct Cpu_s {
    char * architecture;
    uint8_t memory[memory_size];
    uint8_t v[register_size];
    uint16_t stack[pc_size];
    uint16_t pc;
    uint16_t I;
    uint8_t sound;
    uint8_t delay;
    Cycle cycle;
} Cpu_t;
void cycle(Cpu_t *, Frame_t *);
int pop(uint8_t *, uint16_t *);
void push(uint8_t *, uint16_t *, uint16_t);
Cpu_t createEmulator(char *, uint8_t *, uint8_t, uint8_t *, uint16_t);
void destroyEmulator(Cpu_t *);
#endif

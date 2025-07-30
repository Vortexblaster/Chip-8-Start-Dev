#ifndef STD_H_
#define STD_H_
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#endif
#ifndef CORE_H_
#define memory_size 4096
#define address_size 16
#define register_size 16
#define pc_size 12
#define keypad_size 16
#include "SDL3/SDL_keycode.h"
typedef struct Cpu_s Cpu_t;
typedef struct Frame_s Frame_t;
typedef void (*Cycle)(Cpu_t *, Frame_t *);
typedef struct Cpu_s {
    uint8_t memory[memory_size];
    uint8_t v[register_size];
    uint16_t stack[pc_size];
    uint16_t pc;
    uint16_t I;
    uint8_t sound;
    uint8_t delay;
    uint8_t clockSpeedMHz;
    Cycle cycle;
    Frame_t  * processFrame;
    char * architecture;
    SDL_Keycode keypad[keypad_size*2];
    SDL_Keycode currentKey;
} Cpu_t;
#endif
#ifndef VIDEO_H_
#define VIDEO_H_
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_pixels.h"
typedef struct Cpu_s Cpu_t;
typedef struct Frame_s {
    SDL_Surface * frameSurface;
    SDL_Palette * surfacePalette;
    //uint8_t * * screenArray;
    uint16_t resolutionX;
    uint16_t resolutionY;
} Frame_t;
Frame_t createFrame(uint16_t, uint16_t, SDL_Surface *, SDL_Palette *);
void destroyFrame(Frame_t *);
bool initDisplay(SDL_Window * );
bool makeWindow(SDL_Window *, SDL_Surface *, char *);
void run(SDL_Window *, SDL_Surface *, Cpu_t *, Frame_t *);
void cleanupDisplay(SDL_Window *,SDL_Surface *);
#endif

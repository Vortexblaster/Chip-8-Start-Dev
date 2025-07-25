#ifndef STD_H_
#define STD_H_
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#endif
#ifndef VIDEO_H_
#define VIDEO_H_
#include "SDL3/SDL.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_pixels.h"
typedef struct Frame_s {
    SDL_Surface * frameSurface;
    //uint8_t * * screenArray;
    uint16_t resolutionX;
    uint16_t resolutionY;
} Frame_t;
Frame_t createFrame(SDL_Surface * *, uint16_t, uint16_t);
void destroyFrame(Frame_t *);
bool initDisplay(SDL_Window * *);
bool makeWindow(SDL_Window * *, SDL_Surface * *);
void run(SDL_Window * *, SDL_Surface * *, Frame_t *);
void cleanupDisplay(SDL_Window * *,SDL_Surface * *);
#endif

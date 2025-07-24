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
bool initDisplay(SDL_Window * *);
bool makeWindow(SDL_Window * *, SDL_Surface * *);
void run(SDL_Window * * , SDL_Surface * *);
void cleanupDisplay(SDL_Window * *,SDL_Surface * *);
#endif

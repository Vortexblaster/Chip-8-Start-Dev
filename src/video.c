#include "headers/video.h"

SDL_Window * gWindow = NULL;

bool init() {
    bool success = SDL_Init(SDL_INIT_VIDEO);
    if (!success) {
        SDL_Log("Initialization failed: %s\n", SDL_GetError());
    }
    return success;
}

bool makeWindow() {
    const int ScreenWidth = 64;
    const int ScreenHeight = 32;
    gWindow = SDL_CreateWindow("Hello SDL3!", ScreenWidth, ScreenHeight, 0);
    if (gWindow == NULL) {
        SDL_Log("Window creation failed: %s\n", SDL_GetError());
        return false;
    } else {
        return true;
    }
}

void cleanup() {
    SDL_Quit();
}

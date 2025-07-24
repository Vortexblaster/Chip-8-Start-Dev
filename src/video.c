#include "headers/video.h"

bool initDisplay(SDL_Window * *window) {
    bool success = SDL_Init(SDL_INIT_VIDEO);
    if (!success) {
        SDL_Log("Initialization failed: %s\n", SDL_GetError());
    }
    return success;
}

bool makeWindow(SDL_Window * * window, SDL_Surface * * screenSurface) {
    const int screenWidth = 640;
    const int screenHeight = 480;
    *window = SDL_CreateWindow("Hello SDL3!", screenWidth, screenHeight, 0);
    *screenSurface = SDL_GetWindowSurface(*window);
    if (window == NULL) {
        SDL_Log("Window creation failed: %s\n", SDL_GetError());
        return false;
    } else {
        return true;
    }
}

void run(SDL_Window * * window, SDL_Surface * * screenSurface) {
    bool quit = false;
    SDL_Event e;
    SDL_zero(e);
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
        SDL_FillSurfaceRect(*screenSurface, NULL, SDL_MapSurfaceRGB(*screenSurface, 0xFF, 0xFF, 0xFF));
        SDL_UpdateWindowSurface(*window);
    }
}

void cleanupDisplay(SDL_Window * * window, SDL_Surface * * screenSurface) {
    SDL_DestroySurface(*screenSurface); //double free of destroying a surface on a window then the window?
    SDL_DestroyWindow(*window);
    SDL_Quit();
}

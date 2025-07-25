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

void run(SDL_Window * * window, SDL_Surface * * screenSurface, Frame_t * frame) {
    bool quit = false;
    SDL_Event e;
    SDL_zero(e);
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }
        SDL_FillSurfaceRect(*screenSurface, NULL, SDL_MapSurfaceRGB(*screenSurface, 0x0, 0x0, 0x0));

        SDL_UpdateWindowSurface(*window);
    }
}

void cleanupDisplay(SDL_Window * * window, SDL_Surface * * screenSurface) {
    SDL_DestroySurface(*screenSurface); //double free of destroying a surface on a window then the window?
    SDL_DestroyWindow(*window);
    SDL_Quit();
}

Frame_t createFrame(SDL_Surface * *  frameSurface, uint16_t resolutionX, uint16_t resolutionY) {
    Frame_t frame;
    frame.resolutionX = resolutionX;
    frame.resolutionY = resolutionY;
    frame.frameSurface = SDL_CreateSurface(resolutionX, resolutionY, SDL_PIXELFORMAT_INDEX1MSB); //Using this pixel format

//  frame.screenArray = (uint8_t * *) calloc((size_t) frame.resolutionY, sizeof(uint8_t *));
//    for (uint16_t j = 0; j < frame.resolutionY; j++) {
//        frame.screenArray[j] = calloc((size_t) frame.resolutionX, sizeof(uint8_t));
//    }
    return frame;
}

void destroyFrame(Frame_t * frame) { //NULL stack portions of struct and free heap
//    for (uint16_t j = 0; j < frame->resolutionY; j++) {
//        free(frame->screenArray[j]);
//    }
//    free(frame->screenArray);
//    frame->screenArray = NULL;
    frame->resolutionX = 0;
    frame->resolutionY = 0;
    SDL_DestroySurface(frame->frameSurface);
    frame->frameSurface = NULL;
}

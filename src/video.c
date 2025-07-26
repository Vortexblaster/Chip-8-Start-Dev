#include "headers/video.h"

bool initDisplay(SDL_Window * window) {
    bool success = SDL_Init(SDL_INIT_VIDEO);
    if (!success) {
        SDL_Log("Initialization failed: %s\n", SDL_GetError());
    }

    return success;
}

bool makeWindow(SDL_Window * window, SDL_Surface * screenSurface, char * programName) {
    const int screenWidth = 640;
    const int screenHeight = 480;
    window = SDL_CreateWindow(programName, screenWidth, screenHeight, 0);
    screenSurface = SDL_GetWindowSurface(window);
    if (window == NULL) {
        SDL_Log("Window creation failed: %s\n", SDL_GetError());
        return false;
    } else {
        return true;
    }
}

void run(SDL_Window * window, SDL_Surface * screenSurface, Cpu_t * core) {
    bool quit = false;
    SDL_Event e;
    SDL_zero(e);
    while (!quit) {
        SDL_FillSurfaceRect(screenSurface, NULL, SDL_MapSurfaceRGB(screenSurface, 0x0, 0x0, 0x0));
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        for (int i = 0; i < 1; i++) {
                            core->cycle(core);
                            //SDL_BlitSurfaceScaled(core->processFrame->frameSurface, NULL, screenSurface, NULL, SDL_SCALEMODE_NEAREST);
                        }
                        //int h = core->processFrame->frameSurface->h;
                        //int w = core->processFrame->frameSurface->w;
                        if (core->processFrame->frameSurface == NULL) {
                            printf("The surface is NULL!");
                        }
                        //uint8_t * pixels = (uint8_t *) core->processFrame->frameSurface->pixels;
                        //for (int i = 0; i < h*w; i++) {
                            //printf("pixel [%d] : %u", i, pixels[i]);
                            //the problem is this damn surface may not be properly initalized am I falling out of scope?
                        //}
                    }
                    break;
                case SDL_EVENT_KEY_DOWN:
                    SDL_Keycode key = e.key.key;
                    for (int i = 0; i < keypad_size; i++) {
                        if (key == core->keypad[i+i]) {

                        }
                    }
                    break;


            }
        }
        //SDL_BlitSurface(core->processFrame->frameSurface, NULL, screenSurface, NULL);
        SDL_UpdateWindowSurface(window);
    }
}

void cleanupDisplay(SDL_Window * window, SDL_Surface * screenSurface) {
    SDL_DestroySurface(screenSurface); //double free of destroying a surface on a window then the window?
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Frame_t createFrame(uint16_t resolutionX, uint16_t resolutionY) {
    Frame_t frame;
    frame.resolutionX = resolutionX;
    frame.resolutionY = resolutionY;
    frame.frameSurface = SDL_CreateSurface(resolutionX, resolutionY, SDL_PIXELFORMAT_INDEX1LSB); //Using this pixel format maybe MSB dbl check in testing
    if (frame.frameSurface == NULL) {
        SDL_Log("Frame surface creation failed: %s\n", SDL_GetError());
    } else {
        frame.surfacePalette = SDL_CreateSurfacePalette(frame.frameSurface);
    }
//  frame.screenArray = (uint8_t * *) calloc((size_t) frame.resolutionY, sizeof(uint8_t *));
//    for (uint16_t j = 0; j < frame.resolutionY; j++) {
//        frame.screenArray[j] = calloc((size_t) frame.resolutionX, sizeof(uint8_t));
//    }
    if (frame.surfacePalette == NULL) {
        SDL_Log("Surface palette creation failed: %s\n", SDL_GetError());
    }
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
    SDL_DestroySurface(frame->frameSurface); // also takes care of palette
    frame->frameSurface = NULL;
    frame->surfacePalette = NULL;
}

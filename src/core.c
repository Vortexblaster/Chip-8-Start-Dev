#include "headers/video.h"
#include "headers/chip-8.h"
#include "headers/core.h"


int main() {
    //Cpu core = init();
    SDL_Window * window = NULL;
    SDL_Surface * screenSurface = NULL;
    if (!initDisplay(&window)) {
        return -1;
    }
    if (!makeWindow(&window, &screenSurface)) {
        return -2;
    }
    run(&window, &screenSurface);
    cleanupDisplay(&window, &screenSurface);
    window = NULL;
    screenSurface = NULL;
    return 0;
}

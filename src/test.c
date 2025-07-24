#include "SDL3/SDL.h"
#include "headers/test.h"

int main () {
    log_status();
    return 0;
}


void log_status() {
    SDL_Log("%s\n", "This is a test and it was successful!");
}

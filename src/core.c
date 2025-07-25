#include "headers/video.h"
#include "headers/chip-8.h"
#include "headers/core.h"


int main() {
    //Cpu core = init();
    char * architecture = "SUPERC"; //hardcoded for now
    uint8_t font[] = {0xF0, 0x90, 0x90, 0x90, 0xF0,   // 0
                      0x20, 0x60, 0x20, 0x20, 0x70,   // 1
                      0xF0, 0x10, 0xF0, 0x80, 0xF0,   // 2
                      0xF0, 0x10, 0xF0, 0x10, 0xF0,   // 3
                      0x90, 0x90, 0xF0, 0x10, 0x10,   // 4
                      0xF0, 0x80, 0xF0, 0x10, 0xF0,   // 5
                      0xF0, 0x80, 0xF0, 0x90, 0xF0,   // 6
                      0xF0, 0x10, 0x20, 0x40, 0x40,   // 7
                      0xF0, 0x90, 0xF0, 0x90, 0xF0,   // 8
                      0xF0, 0x90, 0xF0, 0x10, 0xF0,   // 9
                      0xF0, 0x90, 0xF0, 0x90, 0x90,   // A
                      0xE0, 0x90, 0xE0, 0x90, 0xE0,   // B
                      0xF0, 0x80, 0x80, 0x80, 0xF0,   // C
                      0xE0, 0x90, 0x90, 0x90, 0xE0,   // D
                      0xF0, 0x80, 0xF0, 0x80, 0xF0,   // E
                      0xF0, 0x80, 0xF0, 0x80, 0x80};  // F
    uint8_t fontByteLength = sizeof(font)/sizeof(font[0]);
    uint16_t programByteLength = 0; //For now
    uint8_t * program = (uint8_t *)calloc(programByteLength, sizeof(uint8_t));
    SDL_Window * window = NULL;
    SDL_Surface * screenSurface = NULL;
    if (!initDisplay(&window)) {
        return -1;
    }
    if (!makeWindow(&window, &screenSurface)) {
        return -2;
    }
    uint16_t resolutionX = 0;
    uint16_t resolutionY = 0;
    SDL_Surface * frameSurface = NULL;
    Frame_t frame = createFrame(&frameSurface, resolutionX, resolutionY); //this will need to be free'd
    Cpu_t core = createEmulator(architecture, font, fontByteLength, program, programByteLength);
    core.cycle(&core, &frame); //that should do the next cycle


    run(&window, &screenSurface, &frame);
    destroyFrame(&frame); //destroy frame before screen
    cleanupDisplay(&window, &screenSurface);
    free(program);
    window = NULL;
    screenSurface = NULL;
    return 0;
}

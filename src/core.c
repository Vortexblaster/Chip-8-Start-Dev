#include "headers/core.h"
#include "headers/video.h"
#include "headers/chip-8.h"


int main() {
    //Cpu core = init();
    FILE * rom = NULL;
    char * architecture = "COSMAC"; //hardcoded for now
    char * programName = "IBM"; //hardcoded for now
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
    SDL_Keycode keypad[] = {SDLK_1,SDLK_1, SDLK_2,SDLK_2, SDLK_3,SDLK_3, SDLK_4,SDLK_C,
                            SDLK_Q,SDLK_4, SDLK_W,SDLK_5, SDLK_E,SDLK_6, SDLK_R,SDLK_D,
                            SDLK_A,SDLK_7, SDLK_S,SDLK_8, SDLK_D,SDLK_9, SDLK_F,SDLK_E,
                            SDLK_Z,SDLK_A, SDLK_X,SDLK_0, SDLK_C,SDLK_B, SDLK_V,SDLK_F};
    uint16_t programByteLength = 0; //For now
    rom = fopen("Chip-8 Pack/Chip-8 Programs/IBM Logo.ch8", "r");
    if (rom == NULL) {
        perror("Error opening file");
        return 1;
    }
    fseek(rom, 0, SEEK_END);         // Go to end
    programByteLength = ftell(rom);  // What is the position?
    rewind(rom);                     // Back to the beginning to read
    uint8_t * program = (uint8_t *)calloc(programByteLength, sizeof(uint8_t));
    for (uint16_t i = 0; i < programByteLength; i++) {
        fread(program+i, 1, 1, rom); //read into array
    }
    //for (uint16_t i = 0; i < programByteLength; i++) {
    //    printf("ROM Byte %u: %hx\n", i , program[i]);
    //}
    SDL_Window * window = NULL;
    SDL_Surface * screenSurface = NULL;
    if (!initDisplay(window)) {
        return -1;
    }
    if (!makeWindow(window, screenSurface, programName)) {
        return -2;
    }
    uint16_t resolutionX = 0;
    uint16_t resolutionY = 0;
    Frame_t frame = createFrame(resolutionX, resolutionY);
    uint8_t clockSpeedMHz = 1;
    Cpu_t core = createEmulator(architecture, clockSpeedMHz, font, fontByteLength, program, programByteLength, keypad, &frame);
    run(window, screenSurface, &core);
    destroyFrame(&frame); //destroy frame before screen
    destroyEmulator(&core);
    cleanupDisplay(window, screenSurface);
    free(program);
    window = NULL;
    screenSurface = NULL;
    return 0;
}

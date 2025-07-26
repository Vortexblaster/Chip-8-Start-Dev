#include "headers/chip-8.h"
#define DEBUG true

void cycle(Cpu_t * core){
    uint8_t top = pc_size;
    uint16_t instruction = core->memory[core->pc] << 0x8 | core->memory[core->pc+1];
    uint16_t immediate_address = instruction & 0x0FFF;
    uint8_t active_register_1 = (instruction & 0x0F00) >> 0x8;
    uint8_t active_register_2 = (instruction & 0x00F0) >> 0x8;
    uint8_t immediate_int = instruction & 0x00FF;
    core->pc += 2;
    if (DEBUG) {
        printf("PC: %u  |  Instruction: %u\n", instruction, core->pc-2);
    }
    switch (instruction & 0xF000) {
        case 0x0:
            switch (instruction & 0x00FF) { //We only have 00XX instructions
                case 0xE0:
                    printf("Clear display");
                    break;
                case 0xEE:
                    core->pc = pop(&top, core->stack);
                    break;
            }
            break;
        case 0x1:
            core->pc = immediate_address;
            break;
        case 0x2:
            push(&top, core->stack, core->pc);
            core->pc = immediate_address;
            break;
        case 0x3:
            if (core->v[active_register_1] == immediate_int) {
                core->pc += 2;
            }
            break;
        case 0x4:
            if (core->v[active_register_1] != immediate_int) {
                core->pc += 2;
            }
            break;
        case 0x5:
            if (core->v[active_register_1] == core->v[active_register_2]) { // maybe add switch here if core->I find another ocore->pcode
                core->pc += 2;
            }
            break;
        case 0x6:
            core->v[active_register_1] = immediate_int;
            break;
        case 0x7:
            core->v[active_register_1] += immediate_int;
            break;
        case 0x8:
            switch (instruction & 0x000F) {
                case 0x0:
                    core->v[active_register_1] = core->v[active_register_2];
                    break;
                case 0x1:
                    core->v[active_register_1] = core->v[active_register_1] | core->v[active_register_2];
                    break;
                case 0x2:
                    core->v[active_register_1] = core->v[active_register_1] & core->v[active_register_2];
                    break;
                case 0x3:
                    core->v[active_register_1] = core->v[active_register_1] ^ core->v[active_register_2];
                    break;
                case 0x4:
                    uint16_t add = core->v[active_register_1] + core->v[active_register_2];
                    if (add > 255) {
                        core->v[0xF] = 1;
                    } else {
                        core->v[0xF] = 0;
                    }
                    core->v[active_register_1] = (uint8_t) add; //double check
                    break;
                case 0x5:
                    if (core->v[active_register_1] > core->v[active_register_2]) {
                        core->v[0xF] = 1;
                    } else {
                        core->v[0xF] = 0;
                    }
                    core->v[active_register_1] = core->v[active_register_1] - core->v[active_register_2];
                    break;
                case 0x6:
                    if (strcmp(core->architecture, "COSMAC")) {
                        core->v[active_register_1] = core->v[active_register_2] >> 0x1;
                        core->v[0xF] = core->v[active_register_2] & 0x1;
                    } else if (strcmp(core->architecture, "SUPERC") || strcmp(core->architecture, "CHIP48")) {
                        core->v[0xF] = core->v[active_register_1] & 0x1;
                        core->v[active_register_1] >>= 0x1;
                    }
                    break;
                case 0x7:
                    if (core->v[active_register_2] > core->v[active_register_1]) {
                        core->v[0xF] = 1;
                    } else {
                        core->v[0xF] = 0;
                    }
                    core->v[active_register_1] = core->v[active_register_2] - core->v[active_register_1];
                    break;
                case 0xE:
                    if (strcmp(core->architecture, "COSMAC") == 0) {
                        core->v[active_register_1] = core->v[active_register_2] << 0x1;
                        core->v[0xF] = core->v[active_register_2] & 0x1;
                    } else if (strcmp(core->architecture, "SUPERC") == 0 || strcmp(core->architecture, "CHIP48") == 0) {
                        core->v[0xF] = core->v[active_register_1] & 0x1;
                        core->v[active_register_1] <<= 0x1;
                    }
            }
            break;
        case 0x9:
            if (core->v[active_register_1] != core->v[active_register_2]) {
                core->pc += 2;
            }
            break;
        case 0xA:
            core->I = immediate_address;
            break;
        case 0xB:
            if (strcmp(core->architecture, "COSMAC") == 0) {
                core->pc = immediate_address + core->v[0x0];
            } else if (strcmp(core->architecture, "SUPERC") == 0 || strcmp(core->architecture, "CHIP48") == 0) {
                core->pc = immediate_address + core->v[active_register_1];
            }
            break;
        case 0xC:
            uint8_t random_num = 0;
            random_num = (uint8_t) rand() % 0xFF;
            core->v[active_register_1] = random_num & immediate_int;
            break;
        case 0xD:
            uint8_t x = core->v[active_register_1] & (uint8_t) (core->processFrame->resolutionX-1);
            uint8_t y = core->v[active_register_2] & (uint8_t) (core->processFrame->resolutionY-1);
            core->v[0xF] = 0;
            uint8_t z = immediate_int & 0x0F; //immediate int holds last byte so mask off MSN
            uint8_t sprite[8];
            memset(sprite, 0, sizeof(sprite));
            uint8_t currentPixelValue = 0;
            uint16_t calculatedIndex = 0;
            if(SDL_MUSTLOCK(core->processFrame->frameSurface)) {
                SDL_LockSurface(core->processFrame->frameSurface);
            }
            uint8_t * pixels = (uint8_t *) core->processFrame->frameSurface->pixels;

            for (uint8_t n = 0; n < z; n++) {
                for (uint8_t pixel = 0; pixel < sizeof(sprite); pixel++) {
                    sprite[pixel] = (core->memory[core->I + n] >> (sizeof(sprite) - pixel)) & 0x1; // & 0x1 to ensure only last bit
                    calculatedIndex = (uint16_t) ((y * core->processFrame->frameSurface->pitch / sizeof(uint8_t)) + x);
                    currentPixelValue = pixels[calculatedIndex] & 0x1;
                    pixels[calculatedIndex] = sprite[pixel] ^ currentPixelValue;
                    if (currentPixelValue == sprite[pixel]) {
                        core->v[0xF] = 1; //set flag
                    }
                    if (x % (core->processFrame->frameSurface->w-1) != 0) {
                        x += 1; //move horizontally
                    } else {
                        break; //reached left edge of screen
                    }
                }
                if (y % (core->processFrame->frameSurface->h-1) != 0) { //may have to change depending on h
                    y += 1; //move vertically
                } else {
                    break; //reached edge of screen
                }
            }
            if(SDL_MUSTLOCK(core->processFrame->frameSurface)) {
                SDL_UnlockSurface(core->processFrame->frameSurface);
            }
            break;
        case 0xE:
            switch (instruction & 0x00FF) {
                case 0x9E:
                    printf("Skip if key pressed"); //implement once display is working
                    break;
                case 0xA1:
                    printf("Skip if key NOT pressed"); //implement once display is working
                    break;
            }
            break;
        case 0xF:
            switch (instruction & 0x00FF) {
                case 0x07:
                    core->v[active_register_1] = core->delay;
                    break;
                case 0x0A:
                    core->pc -= 2; //undo fetch increment
                    printf("Waiting for input");
                    //core->v[active_register_1] = input_value;
                    core->pc += 2;
                    break;
                case 0x15:
                    core->delay = core->v[active_register_1];
                    break;
                case 0x18:
                    core->sound = core->v[active_register_1];
                    break;
                case 0x1E:
                    core->I += core->v[active_register_1];
                    if (strcmp(core->architecture, "AMIGA") == 0) {
                        if (core->I > 0x0FFF) {
                            core->v[0xF] = 1;
                        }
                    }
                    break;
                case 0x29:
                    core->I = 0x50 + (5 * core->v[active_register_1 >> 0x8]); //font range 0x50 - 0x9F
                    break;
                case 0x33:
                    uint8_t value = core->v[active_register_1] % 10;
                    core->v[core->I + 2] = value;
                    value = (core->v[active_register_1] - value) % 10;
                    core->v[core->I + 1] = value;
                    value = (core->v[active_register_1] - value) % 10;
                    core->v[core->I] = value;
                    break;
                case 0x55:
                    if (strcmp(core->architecture, "COSMAC") == 0) {
                        for (int i = 0; i <= active_register_1; i++) {
                            core->memory[core->I] = core->v[i];
                            core->I += 1;
                        }
                        core->I += 1; //We were at I + X -> I + X + 1
                    } else if (strcmp(core->architecture, "SUPERC") == 0 || strcmp(core->architecture, "CHIP48") == 0) {
                        for (int i = 0; i <= active_register_1; i++) {
                            core->memory[core->I + i] = core->v[i];
                        }
                    }
                    break;
                case 0x65:
                     if (strcmp(core->architecture, "COSMAC") == 0) {
                        for (int i = 0; i <= active_register_1; i++) {
                            core->v[i] = core->memory[core->I];
                            core->I += 1;
                        }
                        core->I += 1; //We were at I + X -> I + X + 1
                    } else if (strcmp(core->architecture, "SUPERC") == 0 || strcmp(core->architecture, "CHIP48") == 0) {
                        for (int i = 0; i <= active_register_1; i++) {
                            core->v[i] = core->memory[core->I + i];
                        }
                    }
                    break;
            }
            break;
    }
}

int pop(uint8_t * top, uint16_t * stack) {
    uint16_t popped = 0;
    if (*top == -1){
        printf("Stack underflow?\n");
    } else {
        popped = stack[(*top)-1]; //offset
        stack[(*top)-1] = 0;
        (*top)--;
    }
    return popped;
}

void push(uint8_t * top, uint16_t * stack, uint16_t memory_address) {
    if (*top < pc_size) {
        stack[*top] = memory_address;
        (*top)++;
    }
}

Cpu_t createEmulator(char architecture[], uint8_t clockSpeedMHz, uint8_t font[], uint8_t fontByteLength, uint8_t program[], uint16_t programByteLength, SDL_Keycode * keypad, Frame_t * frame) {
    int seed = time(NULL);
    srand(seed);
    Cpu_t core;
    core.architecture = architecture;
    core.clockSpeedMHz = clockSpeedMHz;
    memset(core.memory, 0, sizeof(core.memory));
    memset(core.v, 0, sizeof(core.v));
    memset(core.stack, 0, sizeof(core.stack));
    memset(core.keypad, 0, sizeof(SDL_Keycode) * keypad_size * 2);
    core.pc = 0x200;
    core.I = 0;
    core.sound = 0;
    core.delay = 0;
    core.cycle = cycle;
    core.processFrame = frame;
    //put load font stuff here
    //font offset = 0x50; //set in main
    for (uint8_t i = 0; i < keypad_size; i++) {
        core.keypad[i] = keypad[i];
        core.keypad[i+1] = keypad[i+1];
    }
    for (uint8_t i = 0; i < fontByteLength; i++) { //make larger for fonts more than 256 bytes long
        core.memory[i + 0x50] = font[i];
    }
    for (uint16_t i = 0; i < programByteLength; i++) {
        core.memory[i + 0x200] = program[i]; //this is how we will load the ROM
    }
    return core;
}

void destroyEmulator(Cpu_t * core) {
    core->architecture = NULL;
    memset(core->memory, 0, sizeof(core->memory));
    memset(core->v, 0, sizeof(core->v));
    memset(core->stack, 0, sizeof(core->stack));
    memset(core->keypad, 0, sizeof(core->keypad));
    core->pc = 0;
    core->I = 0;
    core->sound = 0;
    core->delay = 0;
    core->cycle = cycle;
    core->processFrame = NULL;
    core->currentKey = 0;
}

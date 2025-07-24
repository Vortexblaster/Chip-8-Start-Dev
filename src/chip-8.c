#include "headers/chip-8.h"

void cycle(uint8_t * memory, uint8_t * v, uint16_t * stack, const char * architecture, uint8_t sound, uint8_t delay, uint16_t pc, uint16_t I){
    uint16_t instruction = 0xFFFF;
    uint16_t immediate_address = 0xFFFF;
    uint8_t immediate_int = 0xFF;
    uint8_t random_num = 0xFF;
    uint8_t active_register_1 = 0xFF;
    uint8_t active_register_2 = 0xFF;
    uint8_t top = 0;
    instruction = memory[pc] << 0x8 | memory[pc+1];
    active_register_1 = (instruction & 0x0F00) >> 0x8;
    active_register_2 = (instruction & 0x00F0) >> 0x8;
    immediate_address = instruction & 0x0FFF;
    immediate_int = instruction & 0x00FF;
    switch (instruction & 0xF000) {
        case 0x0:
            switch (instruction & 0x00FF) { //We only have 00XX instructions
                case 0xE0:
                    printf("Clear display");
                    break;
                case 0xEE:
                    pc = pop(&top, stack);
                    break;
            }
            break;
        case 0x1:
            pc = immediate_address;
            break;
        case 0x2:
            push(&top, stack, pc);
            pc = immediate_address;
            break;
        case 0x3:
            if (v[active_register_1] == immediate_int) {
                pc += 2;
            }
            break;
        case 0x4:
            if (v[active_register_1] != immediate_int) {
                pc += 2;
            }
            break;
        case 0x5:
            if (v[active_register_1] == v[active_register_2]) { // maybe add switch here if I find another opcode
                pc += 2;
            }
            break;
        case 0x6:
            v[active_register_1] = immediate_int;
            break;
        case 0x7:
            v[active_register_1] += immediate_int;
            break;
        case 0x8:
            switch (instruction & 0x000F) {
                case 0x0:
                    v[active_register_1] = v[active_register_2];
                    break;
                case 0x1:
                    v[active_register_1] = v[active_register_1] | v[active_register_2];
                    break;
                case 0x2:
                    v[active_register_1] = v[active_register_1] & v[active_register_2];
                    break;
                case 0x3:
                    v[active_register_1] = v[active_register_1] ^ v[active_register_2];
                    break;
                case 0x4:
                    uint16_t add = v[active_register_1] + v[active_register_2];
                    if (add > 255) {
                        v[0xF] = 1;
                    } else {
                        v[0xF] = 0;
                    }
                    v[active_register_1] = (uint8_t) add; //double check
                    break;
                case 0x5:
                    if (v[active_register_1] > v[active_register_2]) {
                        v[0xF] = 1;
                    } else {
                        v[0xF] = 0;
                    }
                    v[active_register_1] = v[active_register_1] - v[active_register_2];
                    break;
                case 0x6:
                    if (strcmp(architecture, "COSMAC")) {
                        v[active_register_1] = v[active_register_2] >> 0x1;
                        v[0xF] = v[active_register_2] & 0x01;
                    } else if (strcmp(architecture, "SUPERC") || strcmp(architecture, "CHIP48")) {
                        v[0xF] = v[active_register_1] & 0x01;
                        v[active_register_1] >>= 0x1;
                    }
                    break;
                case 0x7:
                    if (v[active_register_2] > v[active_register_1]) {
                        v[0xF] = 1;
                    } else {
                        v[0xF] = 0;
                    }
                    v[active_register_1] = v[active_register_2] - v[active_register_1];
                    break;
                case 0xE:
                    if (strcmp(architecture, "COSMAC") == 0) {
                        v[active_register_1] = v[active_register_2] << 0x1;
                        v[0xF] = v[active_register_2] & 0x10;
                    } else if (strcmp(architecture, "SUPERC") == 0 || strcmp(architecture, "CHIP48") == 0) {
                        v[0xF] = v[active_register_1] & 0x10;
                        v[active_register_1] <<= 0x1;
                    }
            }
            break;
        case 0x9:
            if (v[active_register_1] != v[active_register_2]) {
                pc += 2;
            }
            break;
        case 0xA:
            I = immediate_address;
            break;
        case 0xB:
            if (strcmp(architecture, "COSMAC") == 0) {
                pc = immediate_address + v[0x0];
            } else if (strcmp(architecture, "SUPERC") == 0 || strcmp(architecture, "CHIP48") == 0) {
                pc = immediate_address + v[active_register_1];
            }
            break;
        case 0xC:
            random_num = rand() % 0xFF;
            v[active_register_1] = random_num & immediate_int;
            break;
        case 0xD:
            printf("Display");
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
                    v[active_register_1] = delay;
                    break;
                case 0x0A:
                    pc -= 2; //undo fetch increment
                    printf("Waiting for input");
                    //v[active_register_1] = input_value;
                    pc += 2;
                    break;
                case 0x15:
                    delay = v[active_register_1];
                    break;
                case 0x18:
                    sound = v[active_register_1];
                    break;
                case 0x1E:
                    I += v[active_register_1];
                    if (strcmp(architecture, "AMIGA") == 0) {
                        if (I > 0x0FFF) {
                            v[0xF] = 1;
                        }
                    }
                    break;
                case 0x29:
                    I = 0x50 + (5 * v[active_register_1 >> 0x8]); //font range 0x50 - 0x9F
                    break;
                case 0x33:
                    uint8_t value = v[active_register_1] % 10;
                    v[I + 2] = value;
                    value = (v[active_register_1] - value) % 10;
                    v[I + 1] = value;
                    value = (v[active_register_1] - value) % 10;
                    v[I] = value;
                    break;
                case 0x55:
                    if (strcmp(architecture, "COSMAC") == 0) {
                        for (int i = 0; i <= active_register_1; i++) {
                            memory[I] = v[i];
                            I += 1;
                        }
                        I += 1; //We were at I + X -> I + X + 1
                    } else if (strcmp(architecture, "SUPERC") == 0 || strcmp(architecture, "CHIP48") == 0) {
                        for (int i = 0; i <= active_register_1; i++) {
                            memory[I + i] = v[i];
                        }
                    }
                    break;
                case 0x65:
                     if (strcmp(architecture, "COSMAC") == 0) {
                        for (int i = 0; i <= active_register_1; i++) {
                            v[i] = memory[I];
                            I += 1;
                        }
                        I += 1; //We were at I + X -> I + X + 1
                    } else if (strcmp(architecture, "SUPERC") == 0 || strcmp(architecture, "CHIP48") == 0) {
                        for (int i = 0; i <= active_register_1; i++) {
                            v[i] = memory[I + i];
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

Cpu init() {
    int seed = time(NULL);
    srand(seed);
    Cpu core;
    core.cycle = cycle;
    core.pop = pop;
    core.push = push;
    return core;
}

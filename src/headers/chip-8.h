#ifndef STD_H_
#define STD_H_
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#endif
#ifndef CHIP_8_H_
#define CHIP_8_H_
#define memory_size 4096
#define pc_size 12
#define I_size 16
#define address_size 16
#define register_size 16
void cycle(uint8_t *, uint8_t *, uint16_t *, const char *, uint8_t, uint8_t, uint16_t, uint16_t);
int pop(uint8_t *, uint16_t *);
void push(uint8_t *, uint16_t *, uint16_t);
typedef void (*Cycle)(uint8_t *, uint8_t *, uint16_t *, const char *, uint8_t, uint8_t, uint16_t, uint16_t);
typedef int (*Pop)(uint8_t *, uint16_t *);
typedef void (*Push)(uint8_t *, uint16_t *, uint16_t);
typedef struct Cpu {
    uint8_t memory[memory_size];
    uint8_t v[register_size];
    uint16_t stack[pc_size];
    uint16_t pc;
    uint16_t I;
    uint8_t sound;
    uint8_t delay;
    Cycle cycle;
    Pop pop;
    Push push;
} Cpu;
Cpu init();
#endif

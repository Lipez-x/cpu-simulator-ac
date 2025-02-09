
#ifndef CPU_H
#define CPU_H

#define SIZE_MEMORY 0xFFFF

#include <stdint.h>
typedef struct
{
    uint16_t PC = 0x0000;
    uint16_t IR;
    uint16_t SP = 0x8600;
    uint16_t R[8];
    uint8_t C : 1;
    uint8_t Ov : 1;
    uint8_t Z : 1;
    uint8_t S : 1;
} CPU;

void MOV(CPU &cpu);
void ciclo(CPU &cpu);

#endif
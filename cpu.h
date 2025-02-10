
#ifndef CPU_H
#define CPU_H

#define SIZE_MEMORY 0x10000

#include <stdint.h>
typedef struct
{
    uint16_t PC = 0x0000;
    uint16_t IR;
    uint16_t SP = 0x8600;
    uint16_t R[8] = {0};
    bool C;
    bool Ov;
    bool Z;
    bool S;
} CPU;

void ciclo(CPU &cpu);
void MOV(CPU &cpu);
void STORE(CPU &cpu);
void LOAD(CPU &cpu);

void ADD(CPU &cpu);
void SUB(CPU &cpu);
void MUL(CPU &cpu);
void AND(CPU &cpu);

#endif
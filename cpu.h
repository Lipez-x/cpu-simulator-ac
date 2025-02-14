
#ifndef CPU_H
#define CPU_H
#include <cstdio>
#include <stdint.h>

#define SIZE_MEMORY 0x10000
#define SIZE_MEMORY_DATA 0x85F1

typedef struct
{
    uint16_t PC = 0x0000;
    uint16_t IR;
    uint16_t SP = 0x8600;
    uint16_t R[8] = {0};
    bool C = false;
    bool Ov = false;
    bool Z = false;
    bool S = false;
} CPU;

void ciclo(CPU &cpu);

void MOV(CPU &cpu);
void STORE(CPU &cpu);
void LOAD(CPU &cpu);

void ADD(CPU &cpu);
void SUB(CPU &cpu);
void MUL(CPU &cpu);
void AND(CPU &cpu);
void ORR(CPU &cpu);
void NOT(CPU &cpu);
void XOR(CPU &cpu);
void SHR(CPU &cpu);
void SHL(CPU &cpu);
void ROR(CPU &cpu);
void ROL(CPU &cpu);

void CMP(CPU &cpu);
void JMP(CPU &cpu);

void PUSH(CPU &cpu);
void POP(CPU &cpu);

#endif
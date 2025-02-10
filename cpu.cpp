#include "cpu.h"
#include <cstdio>

extern uint8_t memory[SIZE_MEMORY];
extern uint16_t data_memory[SIZE_MEMORY];

void MOV(CPU &cpu)
{
    uint8_t type = cpu.IR >> 11 & 0b1;
    uint8_t Rd = cpu.IR >> 8 & 0b111;

    if (type == 0)
    {
        uint8_t Rm = cpu.IR >> 5 & 0b111;
        printf("MOV R%d, R%X\n", Rd, Rm);
        cpu.R[Rd] = cpu.R[Rm];
    }
    else
    {
        uint8_t Im = cpu.IR & 0xFF;
        printf("MOV R%d, #%X\n", Rd, Im);
        cpu.R[Rd] = Im;
    }
}

void STORE(CPU &cpu)
{
    uint8_t type = cpu.IR >> 11 & 0b1;

    if (type == 0)
    {
        uint8_t Rm = cpu.IR >> 5 & 0b111;
        uint8_t Rn = cpu.IR >> 2 & 0b11;
        printf("STORE [R%d], R%X\n", Rm, Rn);
        data_memory[cpu.R[Rm]] = cpu.R[Rn];
    }
    else
    {
        uint8_t Rm = cpu.IR >> 5 & 0b111;
        uint8_t Im = ((cpu.IR >> 8 & 0b111) << 5) | (cpu.IR & 0b11111);
        printf("STORE [R%d], #%X\n", Rm, Im);
        data_memory[cpu.R[Rm]] = Im;
    }
}

void LOAD(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    printf("LOAD R%d, [R%d]\n", Rd, Rm);
    cpu.R[Rd] = data_memory[cpu.R[Rm]];
}

void ADD(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Rn = cpu.IR >> 2 & 0b111;
    printf("ADD R%d, R%d, R%d\n", Rd, Rm, Rn);
    cpu.R[Rd] = cpu.R[Rm] + cpu.R[Rn];
}

void SUB(CPU &cpu)

{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Rn = cpu.IR >> 2 & 0b111;

    printf("SUB R%d, R%d, R%d\n", Rd, Rm, Rn);
    cpu.R[Rd] = cpu.R[Rm] - cpu.R[Rn];
}

void MUL(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Rn = cpu.IR >> 2 & 0b111;

    printf("MUL R%d, R%d, R%d\n", Rd, Rm, Rn);
    cpu.R[Rd] = cpu.R[Rm] * cpu.R[Rn];
}

void AND(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Rn = cpu.IR >> 2 & 0b111;

    printf("AND R%d, R%d, R%d\n", Rd, Rm, Rn);
    cpu.R[Rd] = cpu.R[Rm] & cpu.R[Rn];
}

void ORR(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Rn = cpu.IR >> 2 & 0b111;

    printf("ORR R%d, R%d, R%d\n", Rd, Rm, Rn);
    cpu.R[Rd] = cpu.R[Rm] | cpu.R[Rn];
}

void ciclo(CPU &cpu)
{
    for (int i = 0; memory[cpu.PC]; i++)
    {
        cpu.IR = memory[cpu.PC] + (memory[cpu.PC + 1] << 8);
        cpu.PC += 2;
        printf("PC: 0x%04X, IR: 0x%04X ", cpu.PC, cpu.IR);

        uint8_t opcode = (cpu.IR >> 12) & 0x0F;
        printf("OPCODE: %1X\n", opcode);

        if (opcode == 0xF)
        {
            break;
        }

        switch (opcode)
        {
        case 0x01:
            MOV(cpu);
            break;
        case 0x02:
            STORE(cpu);
            break;
        case 0x03:
            LOAD(cpu);
            break;
        case 0x04:
            ADD(cpu);
            break;
        case 0x05:
            SUB(cpu);
            break;
        case 0x06:
            MUL(cpu);
            break;
        case 0x07:
            AND(cpu);
            break;
        case 0x08:
            ORR(cpu);
            break;
        default:
            break;
        }
    }
}
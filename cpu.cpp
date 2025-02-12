#include "cpu.h"
#include <cstdio>

extern uint8_t memory[SIZE_MEMORY];
extern uint16_t data_memory[SIZE_MEMORY];
extern uint8_t ultimaInstrucao;
extern bool fimDoArquivo;

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

    cpu.Z = (cpu.R[Rd] == 0); 
    cpu.S = (cpu.R[Rd] & 0x80) != 0; 
    cpu.C = (cpu.R[Rd] > 0xFF); 
    cpu.Ov = ((cpu.R[Rm] ^ cpu.R[Rn]) & 0x80) == 0 && ((cpu.R[Rm] ^ cpu.R[Rd]) & 0x80) != 0; 
}

void SUB(CPU &cpu)

{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Rn = cpu.IR >> 2 & 0b111;

    printf("SUB R%d, R%d, R%d\n", Rd, Rm, Rn);
    cpu.R[Rd] = cpu.R[Rm] - cpu.R[Rn];

    cpu.Z = (cpu.R[Rd] == 0); 
    cpu.S = (cpu.R[Rd] & 0x80) != 0; 
    cpu.C = (cpu.R[Rm] >= cpu.R[Rn]); 
    cpu.Ov = ((cpu.R[Rm] ^ Rn) & 0x80) != 0 && ((cpu.R[Rm] ^ Rd) & 0x80) != 0; 
}

void MUL(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Rn = cpu.IR >> 2 & 0b111;

    printf("MUL R%d, R%d, R%d\n", Rd, Rm, Rn);
    cpu.R[Rd] = cpu.R[Rm] * cpu.R[Rn];

    cpu.Z = (cpu.R[Rd] == 0); 
    cpu.S = (cpu.R[Rd] & 0x80) != 0; 
    cpu.C = (cpu.R[Rd] > 0xFF); 
    cpu.Ov = false; 
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

void NOT(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;

    printf("NOT R%d, R%d\n", Rd, Rm);
    cpu.R[Rd] = ~cpu.R[Rm];
}

void XOR(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Rn = cpu.IR >> 2 & 0b111;

    printf("XOR R%d, R%d, R%d\n", Rd, Rm, Rn);
    cpu.R[Rd] = cpu.R[Rm] ^ cpu.R[Rn];
}

void SHR(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Im = cpu.IR & 0b1111;
    printf("SHR R%d, R%d, #%X\n", Rd, Rm, Im);
    cpu.R[Rd] = cpu.R[Rm] >> Im;
}

void SHL(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    uint8_t Im = cpu.IR & 0b1111;
    printf("SHL R%d, R%d, #%X\n", Rd, Rm, Im);
    cpu.R[Rd] = cpu.R[Rm] << Im;
}

void ROR(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    printf("ROR R%d, R%d\n", Rd, Rm);
    cpu.R[Rd] = (cpu.R[Rm] >> 1) | (cpu.R[Rm] << 15);
}

void ROL(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    uint8_t Rm = cpu.IR >> 5 & 0b111;
    printf("ROL R%d, R%d\n", Rd, Rm);
    cpu.R[Rd] = (cpu.R[Rm] << 1) | (cpu.R[Rm] >> 15);
}

void CMP(CPU &cpu)
{
    int8_t Rm = cpu.IR >> 5 & 0b111;
    int8_t Rn = cpu.IR >> 2 & 0b111;
    printf("CMP R%d, R%d\n", Rm, Rn);

    cpu.Z = (cpu.R[Rm] == cpu.R[Rn]) ? 1 : 0;
    cpu.S = (cpu.R[Rm] < cpu.R[Rn]) ? 1 : 0;
    printf("Ov:%d C:%d Z:%d S:%d\n", cpu.Ov, cpu.C, cpu.Z, cpu.S);
}

void JMP(CPU &cpu)
{
    int16_t Im = cpu.IR >> 2 & 0x1FF;

    if (Im & 0x100)
    {
        Im |= 0xFFE0;
    }

    if ((cpu.IR & 0b11) == 0b00)
    {
        printf("JMP #%X\n", Im);
        cpu.PC = cpu.PC + Im;
    }
    else if ((cpu.IR & 0b11) == 0b01)
    {
        printf("JEQ #%X\n", Im);
        if (cpu.Z == 0 && cpu.S == 0)
        {
            cpu.PC = cpu.PC + Im;
        }
    }
    else if ((cpu.IR & 0b11) == 0b10)
    {
        printf("JLT #%X\n", Im);
        if (cpu.Z == 0 && cpu.S == 1)
        {
            cpu.PC += Im;
        }
    }
    else if ((cpu.IR & 0b11) == 0b11)
    {
        printf("JGT #%X\n", Im);
        if (cpu.Z == 0 && cpu.S == 0)
        {
            cpu.PC += Im;
        }
    }
}

void PUSH(CPU &cpu)
{
    uint8_t Rn = cpu.IR >> 2 & 0b11;
    printf("PSH R%d\n", Rn);
    data_memory[cpu.SP] = cpu.R[Rn];
    cpu.SP--;
}

void POP(CPU &cpu)
{
    uint8_t Rd = cpu.IR >> 8 & 0b111;
    printf("POP R%d\n", Rd);
    cpu.SP++;
    cpu.R[Rd] = data_memory[cpu.SP];
}

void ciclo(CPU &cpu)
{
    for (int i = 0; memory; i++)
    {

        if (cpu.PC == ultimaInstrucao)
        {
            fimDoArquivo = true;
        }

        cpu.IR = memory[cpu.PC] + (memory[cpu.PC + 1] << 8);
        cpu.PC += 2;
        printf("PC: 0x%04X, IR: 0x%04X ", cpu.PC, cpu.IR);

        uint8_t opcode = (cpu.IR >> 12) & 0x0F;
        printf("OPCODE: %1X\n", opcode);
        uint8_t type = cpu.IR >> 11 & 0b1;

        if (opcode == 0xF)
        {
            break;
        }

        switch (opcode)
        {
        case 0x00:
            if (type == 0)
            {
                if ((cpu.IR & 0b11) == 0b11)
                {
                    CMP(cpu);
                }
                else if ((cpu.IR & 0b11) == 0b01)
                {
                    PUSH(cpu);
                }
                else if ((cpu.IR & 0b11) == 0b10)
                {
                    POP(cpu);
                }
            }
            else
            {
                JMP(cpu);
            }
            break;
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
        case 0x09:
            NOT(cpu);
            break;
        case 0x0A:
            XOR(cpu);
            break;
        case 0x0B:
            SHR(cpu);
            break;
        case 0x0C:
            SHL(cpu);
            break;
        case 0x0D:
            ROR(cpu);
            break;
        case 0x0E:
            ROL(cpu);
            break;
        default:
            break;
        }

        if (fimDoArquivo)
        {
            break;
        }
    }
}
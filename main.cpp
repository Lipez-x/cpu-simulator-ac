#include <iostream>
#include <fstream>
#include <sstream>

#define SIZE_MEMORY 0xFFFF
uint8_t memory[SIZE_MEMORY];

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

void MOV(CPU &cpu)
{
    int8_t type = cpu.IR >> 11 & 0b1;
    int8_t Rd = cpu.IR >> 8 & 0b111;

    if (type == 0)
    {
        int8_t Rm = cpu.IR >> 5 & 0b111;
        printf("MOV R%d, R%d\n", Rd, Rm);
        cpu.R[Rd] = cpu.R[Rm];
    }
    else
    {
        int8_t Im = cpu.IR & 0xFF;
        printf("MOV R%d, #%d\n", Rd, Im);
        cpu.R[Rd] = Im;
    }
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
            break;
        default:
            break;
        }
    }
}

void carregarMemoria(char *file_name)
{
    std::ifstream file(file_name);

    if (!file)
    {
        std::cout << "Erro ao abir arquivo\n";
        return;
    }

    std::string line;
    uint16_t address, data;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        char separador;
        if (ss >> std::hex >> address >> separador >> std::hex >> data && separador == ':')
        {
            memory[address] = data;
            memory[++address] = (data >> 8);
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Argumentos inválidos\n";
        return 1;
    }

    carregarMemoria(argv[1]);

    CPU cpu;
    ciclo(cpu);

    for (int i = 0; i < 8; i++)
    {
        printf("R%d: 0x%04X\n", i, cpu.R[i]);
    }
}
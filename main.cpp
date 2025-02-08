#include <iostream>
#include <fstream>
#include <sstream>

#define SIZE_MEMORY 0xFFFF
uint8_t memory[SIZE_MEMORY];

#define HALT 0xFFFF

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

void cicloDeInstrucao(CPU &cpu)
{
    for (int i = 0; i < 30; i++)
    {
        cpu.IR = memory[cpu.PC++] + (memory[cpu.PC++] << 8);
        printf("PC: 0x%04X, IR: 0x%04X\n", cpu.PC, cpu.IR);

        if (cpu.IR == HALT)
        {
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
    while (std::getline(file, line))
    {
        uint16_t address, data;
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
    cicloDeInstrucao(cpu);
}
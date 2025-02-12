#include <iostream>
#include <fstream>
#include <sstream>
#include "cpu.h"

#define SIZE_MEMORY 0x10000
uint8_t memory[SIZE_MEMORY];
uint16_t data_memory[SIZE_MEMORY] = {0};

uint8_t ultimaInstrucao;
bool fimDoArquivo;

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

    ultimaInstrucao = address - 1;
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

    printf("PC: 0x%04X, SP: 0x%04X\n", cpu.PC, cpu.SP);
    for (int i = 0; i < 8; i++)
    {
        printf("R%d: 0x%04X\n", i, cpu.R[i]);
    }
    for (int i = 0; i < SIZE_MEMORY; i++)
    {
        if (data_memory[i])
            printf("%04X: 0x%04X\n", i, data_memory[i]);
    }

    printf("Ov:%d C:%d Z:%d S:%d\n", cpu.Ov, cpu.C, cpu.Z, cpu.S);
}
#include <iostream>
#include <fstream>
#include <sstream>
#include "cpu.h"

#define SIZE_MEMORY 0xFFFF
uint8_t memory[SIZE_MEMORY];

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
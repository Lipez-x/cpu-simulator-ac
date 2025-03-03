#include <iostream>
#include <fstream>
#include <sstream>
#include "cpu.h"

#define SIZE_MEMORY 0x10000
uint8_t memory[SIZE_MEMORY];
uint8_t data_memory[SIZE_MEMORY_DATA] = {0};
bool memory_accessed[SIZE_MEMORY_DATA];
uint8_t stack[16] = {0};

uint8_t ultimaInstrucao;
bool fimDoArquivo;

void carregarMemoria(char *file_name)
{
    std::ifstream file(file_name);

    for (int i = 0; i < SIZE_MEMORY; i++)
    {
        memory[i] = 0xFF;
    }

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
}
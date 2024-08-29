#ifndef CPU_H
#define CPU_H

#include <stdbool.h>
#include "programa.h"

typedef struct sis Computador;

// Processador único
typedef struct {
    Computador *sis;      // referência para o mundo externo
    Programa *codigo;     // referência para as instruções do processo atual
    int pidProcessoAtual; // ID do processo atual (mover na tabela de processos)
    int pc;               // Program Counter (instrução que o processo atual está)
    int quantum;          // tempo max de execução de um processo (em unidades)
} CPU;

// Inicializa a CPU com referências à módulos externos necessários à sua operação
void cpuInicia(CPU *cpu, Computador *sis);

// Carrega um processo na CPU pelo ID, com quantum fixo
void cpuCarregaProcesso(CPU *cpu, int pidProcessoAtual);

// Executa a próxima instrução do processo atual
int cpuExecutaProximaInstrucao(CPU *cpu, int tempo);

// Checa se não há nenhum processo carregado
bool cpuEstaLivre(const CPU *cpu);

void cpuImprime(const CPU *cpu, int numeroCPU, int tempo);

#endif // CPU_H

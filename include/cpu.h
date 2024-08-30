#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

#include "bloqueados.h"
#include "escalonamento.h"
#include "memoria.h"
#include "tabela.h"

// Processador único
typedef struct {
    // Referências para módulos externos
    Memoria *mem;
    TabelaProcessos *tabela;
    ListaBloqueados *bloq;
    Escalonador esc;

    Processo *proc;       // referência ao processo atual
    Programa *codigo;     // referência ao código do processo atual
    int pidProcessoAtual; // ID do processo atual (mover na tabela de processos)
    int pc;               // Program Counter (instrução que o processo atual está)
    int quantum;          // tempo max de execução de um processo (em unidades)
} CPU;

// Inicializa a CPU com referências à módulos externos necessários à sua operação
void cpuInicia(CPU *cpu, Memoria *mem, TabelaProcessos *tabela,
        ListaBloqueados *bloq, Escalonador esc);

// Carrega um processo na CPU pelo ID, com quantum fixo
void cpuCarregaProcesso(CPU *cpu, int pidProcessoAtual);

// Executa a próxima instrução do processo atual
int cpuExecutaProximaInstrucao(CPU *cpu, int tempo);

// Checa se não há nenhum processo carregado
bool cpuEstaLivre(const CPU *cpu);

void cpuImprime(const CPU *cpu, int numeroCPU, int tempo);

#endif // CPU_H

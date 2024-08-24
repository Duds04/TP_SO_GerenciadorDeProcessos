#ifndef CPU_H
#define CPU_H

#include "tabela.h"
#include "instrucao.h"
#include "bloqueados.h"
#include "escalonamento.h"
#include <stdbool.h>

// Processador único
typedef struct {
    TabelaProcessos *tabela;          // referência para a tabela de processos
    ListaBloqueados *listaBloqueados; // lista de processos bloqueados
    Escalonador esc;                  // escalonador

    int pidProcessoAtual; // ID do processo atual (mover na tabela de processos)
    int pc;               // Program Counter (instrução que o processo atual está)
    int quantum;          // tempo max de execução de um processo (em unidades)
    int *reg, numRegs;    // registradores do processo atual
    Programa *codigo;     // referência para as instruções do processo atual
} CPU;

// Inicializa a CPU com referências à módulos externos necessários à sua operação
void cpuInicia(CPU *cpu, TabelaProcessos *tab,
        ListaBloqueados *listaBloqueados, Escalonador esc);

// Carrega um processo na CPU pelo ID, com quantum fixo
void cpuCarregaProcesso(CPU *cpu, int pidProcessoAtual);

// Executa a próxima instrução do processo atual
int cpuExecutaProximaInstrucao(CPU *cpu, int tempo);

// Checa se não há nenhum processo carregado
bool cpuEstaLivre(const CPU *cpu);

void cpuImprime(const CPU *cpu, int numeroCPU, int tempo);

#endif // CPU_H

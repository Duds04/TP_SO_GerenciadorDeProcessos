#ifndef CPU_H
#define CPU_H

#include <stdbool.h>

#include "tabela.h"
#include "instrucao.h"
#include "bloqueados.h"
#include "escalonamento.h"

// Estado atual da CPU
typedef struct {
    TTabelaProcesso *pTabela;         // referência para a tabela de processos
    ListaBloqueados *listaBloqueados; // lista de processos bloqueados
    void *escalonador;                // referência para a estrutura do escalonador
    Escalonamento esc;                // identificação do algoritmo de escalonamento

    int tempo;               // contador de tempo
    int pidProcessoAtual;    // ID do processo atual (mover na tabela de processos)
    int pc;                  // Program Counter (instrução que o processo atual está)
    int quantum;             // tempo max de execução de um processo (em unidades)
    int *reg, num_regs;      // registradores do processo atual
    TListaInstrucao *codigo; // referência para as instruções do processo atual

    // PfilasPrioridades filas;
} CPU;

// Inicializa a CPU com referências à módulos externos necessários à sua operação
void inicializaCPU(CPU *cpu, TTabelaProcesso *pLista,
        ListaBloqueados *listaBloqueados, void *escalonador, Escalonamento esc);

// Carrega um processo na CPU pelo ID, com quantum fixo
void carregaProcesso(CPU *cpu, int pidProcessoAtual);

// Executa a próxima instrução do processo atual
void executaProximaInstrucao(CPU *cpu);

// Checa se não há nenhum processo carregado
bool cpuIsLivre(const CPU *cpu);

// Deixa a CPU zerada
void zeraCPU(CPU *cpu);

#endif // CPU_H

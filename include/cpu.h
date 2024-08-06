#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "processo.h"
#include "tabela.h"
#include "bloqueados.h"


typedef struct 
{
    int pidProcessoAtual; // ID do processo atual (mover na tabela de processos)
    int PC; // Program Counter (instrução que o processo atual está)
    int Quantum; // Tempo de max de execução de um processo
    TTabelaProcesso* pTabela; // Estado da CPU
    int *reg, num_regs;     // registradores do processo atual
    ListaBloqueados* listaBloqueados; // Lista de processos bloqueados
} CPU;


void inicializaCPU(CPU*cpu, TTabelaProcesso *pLista, ListaBloqueados* listaBloqueados);
void carregaProcesso(CPU* cpu, int pidProcessoAtual, int QuantumProcessoAtual);
void executaProximaInstrucao(CPU* cpu); // será usada para o processo U, no gerenciador


int cpuIsLivre(CPU* cpu); // retorna se o processo atual está vazio
void zeraCPU(CPU* cpu);


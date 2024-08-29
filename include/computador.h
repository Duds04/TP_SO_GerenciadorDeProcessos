#ifndef COMPUTADOR_H
#define COMPUTADOR_H

#include <stdbool.h>
#include <stdint.h>

#include "cpu.h"
#include "tabela.h"
#include "memoria.h"
#include "bloqueados.h"
#include "escalonamento.h"

// Estrutura que representa um computador, com múltiplas CPUs
typedef struct sis {
    CPU *cpus; int numCPUs; // múltiplas CPUs
    Memoria mem;            // memória principal
    TabelaProcessos tabela; // tabela de processos
    ListaBloqueados bloq;   // estrutura de controle de processos bloqueados
    Escalonador esc;        // algoritmo de escalonamento
    int tempo;              // contador de tempo (em unidades)
} Computador;

// Inicializa o computador
void computadorInicia(Computador *sis, int numCPUs, EscalonamentoID escId,
        AlocID alocId, Programa init);

// Executa próxima unidade de tempo, retornando um valor negativo caso não
// hajam mais processos no sistema a serem executados
int computadorExecuta(Computador *sis);

// Retorna o tempo médio de resposta de todos os processos
double computadorTempoMedio(const Computador *sis);

// Imprime os dados do computador
void computadorImprime(const Computador *sis);

// Acessa uma posição de memória (variável inteira) relativa a um processo atual
int32_t *computadorAcessa(Computador *sis, int pid, int i);

// Desaloca a memória associada ao computador
void computadorLibera(Computador *sis);

#endif // COMPUTADOR_H

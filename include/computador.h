#ifndef MULTIPLASCPUS_H
#define MULTIPLASCPUS_H

#include <stdbool.h>

#include "cpu.h"
#include "tabela.h"
#include "bloqueados.h"
#include "escalonamento.h"

// Estrutura que representa um computador, com múltiplas CPUs
typedef struct {
    CPU *cpus;
    int numCPUs;
} Computador;

// Inicializa o computador (TODO refatorar isso, parâmetros demais)
void computadorInicia(Computador* c, int numCPUs, TabelaProcessos *tabela,
        ListaBloqueados *bloq, void *escalonador, Escalonamento esc);

// Executa próxima instrução em cada CPU
void computadorExecuta(Computador *c);

void computadorImprime(const Computador* c);

void computadorLibera(Computador* c);

#endif // MULTIPLASCPUS_H


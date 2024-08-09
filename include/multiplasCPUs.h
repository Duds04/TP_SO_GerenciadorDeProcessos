#ifndef MULTIPLASCPUS_H
#define MULTIPLASCPUS_H

#include <stdbool.h>

#include "cpu.h"
#include "tabela.h"
#include "bloqueados.h"
#include "escalonamento.h"

typedef struct {
    int numCPUs; // numero de CPUs
    CPU* cpus;   // Lista de CPUs
} MultiCPUs;

// Inicializa as múltiplas CPUs
void iniciaMultiCPUs(MultiCPUs* multiCPUs, int numCPUs, TTabelaProcesso *tabela,
        ListaBloqueados *bloq, void *escalonador, Escalonamento esc);

void executaProximaInstrucaoMulti(MultiCPUs *cpu);

void liberaMultiCPUs(MultiCPUs* multiCPUs);

#endif // MULTIPLASCPUS_H


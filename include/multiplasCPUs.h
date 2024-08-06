#ifndef MULTIPLASCPUS_H
#define MULTIPLASCPUS_H

#include <stdbool.h>

#include "cpu.h"

typedef struct {
    int numCPUs; // numero de CPUs
    CPU* cpus;   // Lista de CPUs
} MultiCPUs;

void iniciaMultiCPUs(MultiCPUs* multiCPUs, int numCPUs, CPU* cpus);
void executaProximaInstrucao(MultiCPUs *cpu, void *Escalonador);
void liberaMultiCPUs(MultiCPUs* multiCPUs);

#endif // MULTIPLASCPUS_H


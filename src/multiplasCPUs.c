#include <stdlib.h>

#include "cpu.h"
#include "multiplasCPUs.h"


void iniciaMultiCPUs(MultiCPUs* multiCPUs, int numCPUs, TTabelaProcesso *tabela,
        ListaBloqueados *bloq, void *escalonador, Escalonamento esc) {
    multiCPUs->numCPUs = numCPUs;
    multiCPUs->cpus = (CPU*) malloc(numCPUs * sizeof(CPU));
    for (int i = 0; i < numCPUs; i++) {
        inicializaCPU(&multiCPUs->cpus[i], tabela, bloq, escalonador, esc);
    }
}

void executaProximaInstrucaoMulti(MultiCPUs *cpu) {
    for (int i = 0; i < cpu->numCPUs; i++)
        executaProximaInstrucao(&cpu->cpus[i]);
}

void liberaMultiCPUs(MultiCPUs* multiCPUs){
    free(multiCPUs->cpus);
    multiCPUs->numCPUs = 0;
}

void imprimeMultiCPUs(MultiCPUs* multiCPUs){
    for (int i = 0; i < multiCPUs->numCPUs; i++) {
        imprimeCPU(&multiCPUs->cpus[i]);
    }
}
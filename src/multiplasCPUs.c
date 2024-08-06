#include "multiplasCPUs.h"


void iniciaMultiCPUs(MultiCPUs* multiCPUs, int numCPUs) {
    multiCPUs->numCPUs = numCPUs;
    multiCPUs->cpus = (CPU*) malloc(numCPUs * sizeof(CPU));
    for (int i = 0; i < numCPUs; i++) {
        zeraCPU(&multiCPUs->cpus[i]);
    }
}

void executaProximaInstrucao(MultiCPUs *cpu, void *Escalonador){
    for (int i = 0; i < cpu->numCPUs; i++) {
        executaProximaInstrucao(&cpu->cpus[i], Escalonador);
    }
}

void liberaMultiCPUs(MultiCPUs* multiCPUs){
    multiCPUs->cpus = NULL;
    multiCPUs->numCPUs = 0;
}


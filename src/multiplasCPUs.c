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
    for (int i = 0; i < cpu->numCPUs; i++){
        fprintf(stderr, "CPU #%d ", i+1);
        if(executaProximaInstrucao(&cpu->cpus[i])){
            printf("Ociosa\n");
        }
    }
}

void liberaMultiCPUs(MultiCPUs* multiCPUs){
    free(multiCPUs->cpus);
    multiCPUs->numCPUs = 0;
}

void imprimeMultiCPUs(const MultiCPUs* cpus){
    for(int i = 0; i < cpus->numCPUs; ++i) {
        if(cpus->cpus[i].pidProcessoAtual < 0) continue;
        printf("\tCPU #%d:\n------------------\n", i + 1);
        imprimeCPU(&cpus->cpus[i], i);
        
    }
}

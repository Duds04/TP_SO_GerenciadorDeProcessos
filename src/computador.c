#include <stdlib.h>
#include "cpu.h"
#include "computador.h"

// Inicializa o computador (TODO refatorar isso, parâmetros demais)
void computadorInicia(Computador* c, int numCPUs, TabelaProcessos *tabela,
        ListaBloqueados *bloq, void *escalonador, Escalonamento esc) {
    c->numCPUs = numCPUs;
    c->cpus = (CPU*) malloc(numCPUs * sizeof(CPU));
    for (int i = 0; i < numCPUs; i++) {
        cpuInicia(&c->cpus[i], tabela, bloq, escalonador, esc);
    }
}

// Executa próxima instrução em cada CPU
void computadorExecuta(Computador *c) {
    for (int i = 0; i < c->numCPUs; i++) {
        fprintf(stderr, "CPU #%d ", i+1);
        if(cpuExecutaProximaInstrucao(&c->cpus[i])){
            printf("Ociosa\n");
        }
    }
}

void computadorImprime(const Computador* c) {
    for(int i = 0; i < c->numCPUs; ++i) {
        if(c->cpus[i].pidProcessoAtual < 0) continue;
        printf("\tCPU #%d:\n------------------\n", i + 1);
        cpuImprime(&c->cpus[i], i);
    }
}

void computadorLibera(Computador* c) {
    free(c->cpus);
    c->numCPUs = 0;
}

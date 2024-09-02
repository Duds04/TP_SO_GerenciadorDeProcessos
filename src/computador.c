#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cpu.h"
#include "memoria.h"
#include "tabela.h"
#include "computador.h"
#include "bloqueados.h"
#include "escalonamento.h"

void computadorInicia(Computador *sis, int numCPUs, EscalonamentoID escId,
        AlocID alocId, Programa init) {
    // Alocando as CPUs
    sis->numCPUs = numCPUs;
    sis->cpus = (CPU*) malloc(numCPUs * sizeof(CPU));
    if(sis->cpus == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    sis->tempo = 0;
    memoriaInicia(&sis->mem, alocId);
    escalonadorInicia(&sis->esc, escId);
    tabelaProcessosInicia(&sis->tabela);
    bloqueadosInicia(&sis->bloq);

    // Inserindo o programa inicial na tabela e no escalonamento
    int id = tabelaProcessosAdiciona(&sis->tabela, -1, 0, init, 0);
    escalonadorAdiciona(sis->esc, tabelaProcessosAcessa(&sis->tabela, id));

    // Inicializando as CPUs
    for(int i = 0; i < numCPUs; ++i)
        cpuInicia(&sis->cpus[i], &sis->mem, &sis->tabela,
                &sis->bloq, sis->esc);
}

// Retorna o tempo médio de resposta de todos os processos
double computadorTempoMedio(const Computador *sis) {
    return sis->tempo / (double) sis->tabela.contadorTodosProcessos;
}

// Executa próxima unidade de tempo, retornando um valor negativo caso não
// hajam mais processos no sistema a serem executados
int computadorExecuta(Computador *sis) {
    if(sis->tabela.contadorProcessos == 0) return -1;

    // Checamos se há algum processo a desbloquear
    int id;
    bloqueadosTique(&sis->bloq);
    while((id = bloqueadosRemove0(&sis->bloq)) >= 0)
        // Processos desbloqueados são reinseridos no escalonador
        escalonadorAdiciona(sis->esc, tabelaProcessosAcessa(&sis->tabela, id));

    ++sis->tempo;
    for (int i = 0; i < sis->numCPUs; i++) {
        fprintf(stderr, "CPU #%d ", i + 1);
        if(cpuExecutaProximaInstrucao(&sis->cpus[i], sis->tempo))
            printf("Ociosa\n");
    }
    return 0;
}

void computadorImprime(const Computador* sis) {
    printf("\n\tIMPRIMINDO DADOS DA MEMÓRIA\n");
    memoriaImprime(&sis->mem);
    printf("\n\tIMPRIMINDO DADOS DA(S) CPU(S):\n");
    for(int i = 0; i < sis->numCPUs; ++i) {
        if(sis->cpus[i].pidProcessoAtual < 0) continue;
        printf("\tCPU #%d:\n------------------\n", i + 1);
        cpuImprime(&sis->cpus[i], i, sis->tempo);
    }

    printf("\n\tIMPRIMINDO DADOS DO ESCALONADOR: \n");
    escalonadorImprime(sis->esc);

    printf("\n\tIMPRIMINDO DADOS DA TABELA DE PROCESSOS: \n");
    tabelaProcessosImprime(&sis->tabela, &sis->mem);

    printf("\n\tIMPRIMINDO DADOS DA LISTA DE BLOQUEADOS:\n");
    bloqueadosImprime(&sis->bloq);
}

// Desaloca a memória associada ao computador
void computadorLibera(Computador* sis) {
    free(sis->cpus);
    sis->numCPUs = 0;
    sis->tempo = 0;
    tabelaProcessosLibera(&sis->tabela);
    bloqueadosLibera(&sis->bloq);
    escalonadorLibera(sis->esc);
}

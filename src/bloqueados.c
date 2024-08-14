#include <stdio.h>
#include <stdlib.h>
#include "bloqueados.h"

#define CAP_INICIAL 16

// Inicializa lista de bloqueados
void bloqueadosInicia(ListaBloqueados *bloq) {
    bloq->processos = (ProcessoBloqueado*) malloc(CAP_INICIAL * sizeof(ProcessoBloqueado));
    if(bloq->processos == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    bloq->tam = 0;
    bloq->capacidade = CAP_INICIAL;
}

// Insere novo processo bloqueado na lista
void bloqueadosAdiciona(ListaBloqueados *bloq, int pid, int tempo_restante) {
    if(bloq->tam + 1 > bloq->capacidade) {
        bloq->processos = realloc(bloq->processos,
                bloq->capacidade * 2 * sizeof(ProcessoBloqueado));
        if(bloq->processos == NULL) {
            fprintf(stderr, "[!] Sem memória suficiente\n");
            exit(64);
        }
        bloq->capacidade *= 2;
    }
    ProcessoBloqueado p = { .pid = pid, .tempoRestante = tempo_restante };
    bloq->processos[bloq->tam++] = p;
}

// Imprime a lista de bloqueados
void bloqueadosImprime(const ListaBloqueados *bloq) {
    printf("PID TEMPO\n-----------\n");
    for(int i = 0; i < bloq->tam; ++i)
        printf("%02d %03d\n", bloq->processos[i].pid,
                bloq->processos[i].tempoRestante);
}

// Decrementa o tempo restante em todos os processos por uma unidade. Todos os
// processos onde o tempo restante torna-se zero (i.e. com desbloqueio pendente)
// são movidos para o fim da lista
void bloqueadosTique(ListaBloqueados *bloq) {
    int i = 0, final = bloq->tam - 1;
    // Antes de mais nada, é preciso garantir que os processos que já estão com
    // tempo zerado e no fim da lista estejam fora de consideração
    while(final >= 0 && bloq->processos[final].tempoRestante == 0) --final;
    if(final < 0) return; // nada a fazer

    while(i <= final) {
        if(bloq->processos[i].tempoRestante <= 0)
            continue; // nem deveria acontecer
        bloq->processos[i].tempoRestante -= 1;
        if(bloq->processos[i].tempoRestante == 0) {
            // Processo com desbloqueio pendente; deve ser movido para o fim da
            // lista, para uma operação remove0 futura
            ProcessoBloqueado aux = bloq->processos[final];
            bloq->processos[final] = bloq->processos[i];
            bloq->processos[i] = aux;
            --final; // menos um processo a considerar
       } else {
           ++i; // passamos para o próximo processo
       }
    }
}

// Remove um processo bloqueado com tempo restante igual a zero da lista,
// retornando seu ID. Caso não haja nenhum processo com tempo restante zerado,
// retorna -1. Invoque repetidas vezes para desbloquear todos os processos com
// desbloqueio pendente
int bloqueadosRemove0(ListaBloqueados *bloq) {
    if(bloq->tam == 0) return -1;
    if(bloq->processos[bloq->tam - 1].tempoRestante > 0) return -1;
    return bloq->processos[--bloq->tam].pid;
}

// Desaloca a memória para a lista de bloqueados
void bloqueadosLibera(ListaBloqueados *bloq) {
    free(bloq->processos);
    bloq->processos = NULL;
    bloq->capacidade = 0;
    bloq->tam = 0;
}

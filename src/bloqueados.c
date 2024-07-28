#include <stdio.h>
#include <stdlib.h>
#include "bloqueados.h"

#define CAP_INICIAL 16

// Inicializa lista de bloqueados
void bloqueados_inicia(ListaBloqueados *lb) {
    lb->processos = (ProcessoBloqueado*) malloc(CAP_INICIAL * sizeof(ProcessoBloqueado));
    if(lb->processos == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    lb->tam = 0;
    lb->capacidade = CAP_INICIAL;
}

// Insere novo processo bloqueado na lista
void bloqueados_insere(ListaBloqueados *lb, int pid, int tempo_restante) {
    if(lb->tam + 1 > lb->capacidade) {
        lb->processos = realloc(lb->processos,
                lb->capacidade * 2 * sizeof(ProcessoBloqueado));
        if(lb->processos == NULL) {
            fprintf(stderr, "[!] Sem memória suficiente\n");
            exit(64);
        }
        lb->capacidade *= 2;
    }
    ProcessoBloqueado p = { .pid = pid, .tempo_restante = tempo_restante };
    lb->processos[lb->tam++] = p;
}

// Imprime a lista de bloqueados
void bloqueados_imprime(const ListaBloqueados *lb) {
    printf("PID TEMPO\n---------\n");
    for(int i = 0; i < lb->tam; ++i)
        printf("%02d %03d\n", lb->processos[i].pid,
                lb->processos[i].tempo_restante);
}

// Decrementa o tempo restante em todos os processos por uma unidade. Todos os
// processos onde o tempo restante torna-se zero (i.e. com desbloqueio pendente)
// são movidos para o fim da lista
void bloqueados_tique(ListaBloqueados *lb) {
    int i = 0, final = lb->tam - 1;
    // Antes de mais nada, é preciso garantir que os processos que já estão com
    // tempo zerado e no fim da lista estejam fora de consideração
    while(final >= 0 && lb->processos[final].tempo_restante == 0) --final;
    if(final < 0) return; // nada a fazer

    while(i <= final) {
        if(lb->processos[i].tempo_restante <= 0)
            continue; // nem deveria acontecer
        lb->processos[i].tempo_restante -= 1;
        if(lb->processos[i].tempo_restante == 0) {
            // Processo com desbloqueio pendente; deve ser movido para o fim da
            // lista, para um operação remove0 futura
            ProcessoBloqueado aux = lb->processos[final];
            lb->processos[final] = lb->processos[i];
            lb->processos[i] = aux;
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
int bloqueados_remove0(ListaBloqueados *lb) {
    if(lb->tam == 0) return -1;
    if(lb->processos[lb->tam - 1].tempo_restante > 0) return -1;
    return lb->processos[--lb->tam].pid;
}

// Desaloca a memória para a lista de bloqueados
void bloqueados_libera(ListaBloqueados *lb) {
    free(lb->processos);
    lb->processos = NULL;
    lb->capacidade = 0;
    lb->tam = 0;
}

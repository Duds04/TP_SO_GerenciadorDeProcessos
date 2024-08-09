#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "robin.h"

// BitWise, substitui mod (dado que capacidade é potencia de 2)
#define INC_MOD(i, cap) (((i) + 1) & ((cap) - 1))

void robin_inicia(FilaRobin *r) {
    r->id_processos = (int*) malloc(sizeof(int) * CAP_INICIAL);
    if(r->id_processos == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    r->primeiro = 0;
    r->ultimo = 0;
    r->capacidade = CAP_INICIAL;
}

void robin_libera(FilaRobin *r) {
    free(r->id_processos);
    r->primeiro = r->ultimo = 0;
    r->capacidade = 0;
}

static void robin_aumenta(FilaRobin *r) {
    int count = 0;
    int* aux = (int*) malloc(sizeof(int) * r->capacidade * 2);
    if(aux == NULL) {
        fprintf(stderr, "[!] Realocação robin falhada\n");
        exit(64);
    }

    for(int i = r->primeiro; i != r->ultimo; i = INC_MOD(i, r->capacidade)){
        aux[count] = r->id_processos[i];
        count++;
    }
    free(r->id_processos);
    r->id_processos = aux;

    r->capacidade *= 2;
    r->primeiro = 0;
    r->ultimo = count;
}

int robin_retira(FilaRobin *r) {
    if(robin_vazio(r)) return -1;
    int escolhido = r->id_processos[r->primeiro];
    r->primeiro = INC_MOD(r->primeiro, r->capacidade);
    return escolhido;
}

static bool robin_cheio(const FilaRobin *r) {
    return r->primeiro == INC_MOD(r->ultimo, r->capacidade);
}

void robin_adiciona(FilaRobin *r, int id_processo) {
    // verifica se está cheio
    if(robin_cheio(r))
        robin_aumenta(r);
    r->id_processos[r->ultimo] = id_processo;
    r->ultimo = INC_MOD(r->ultimo, r->capacidade);
}

// Checa se a fila está vazia
bool robin_vazio(const FilaRobin *r) {
    return r->primeiro == r->ultimo;
}

void robin_imprime(const FilaRobin *r){
    if(robin_vazio(r)) {
        printf("[]\n");
        return;
    }
    printf("[%d", r->id_processos[r->primeiro]);
    int segundo = INC_MOD(r->primeiro, r->capacidade);
    for(int i = segundo; i != r->ultimo; i = INC_MOD(i, r->capacidade)){
        printf(" %d", r->id_processos[i]);
    }
    printf("]\n");
}

#undef INC_MOD

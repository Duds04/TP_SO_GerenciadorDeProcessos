#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fila.h"

// BitWise, substitui mod (dado que capacidade é potencia de 2)
#define INC_MOD(i, cap) (((i) + 1) & ((cap) - 1))

// Inicializa fila
void filaInicia(FilaID *fila) {
    fila->idProcessos = (int*) malloc(sizeof(int) * CAP_INICIAL);
    if(fila->idProcessos == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    fila->primeiro = 0;
    fila->ultimo = 0;
    fila->capacidade = CAP_INICIAL;
}

// Dobra a capacidade de memória associada à fila
static void aumentaFila(FilaID *fila) {
    int count = 0;
    int *aux = (int*) malloc(sizeof(int) * fila->capacidade * 2);
    if(aux == NULL) {
        fprintf(stderr, "[!] Realocação de fila falhou\n");
        exit(64);
    }
    for(int i = fila->primeiro; i != fila->ultimo;
            i = INC_MOD(i, fila->capacidade)) {
        aux[count] = fila->idProcessos[i];
        count++;
    }
    free(fila->idProcessos);
    fila->idProcessos = aux;
    fila->capacidade *= 2;
    fila->primeiro = 0;
    fila->ultimo = count;
}

// Checa se a fila está cheia
static bool filaEstaCheia(const FilaID *fila) {
    return fila->primeiro == INC_MOD(fila->ultimo, fila->capacidade);
}

// Adiciona ID de processo à fila
void filaAdiciona(FilaID *fila, int idProcesso) {
    // verifica se está cheio
    if(filaEstaCheia(fila))
        aumentaFila(fila);
    fila->idProcessos[fila->ultimo] = idProcesso;
    fila->ultimo = INC_MOD(fila->ultimo, fila->capacidade);
}

// Remove ID da fila e retorna-o
int filaRemove(FilaID *fila) {
    if(filaEstaVazia(fila)) return -1;
    int escolhido = fila->idProcessos[fila->primeiro];
    fila->primeiro = INC_MOD(fila->primeiro, fila->capacidade);
    return escolhido;
}

// Imprime a fila de forma legível
void filaImprime(const FilaID *fila) {
    if(filaEstaVazia(fila)) {
        printf("[]\n");
        return;
    }
    printf("[%d", fila->idProcessos[fila->primeiro]);
    int segundo = INC_MOD(fila->primeiro, fila->capacidade);
    for(int i = segundo; i != fila->ultimo; i = INC_MOD(i, fila->capacidade)){
        printf(" %d", fila->idProcessos[i]);
    }
    printf("]\n");
}

// Checa se a fila está vazia
bool filaEstaVazia(const FilaID *fila) {
    return fila->primeiro == fila->ultimo;
}

// Desaloca a fila
void filaLibera(FilaID *fila) {
    free(fila->idProcessos);
    fila->primeiro = fila->ultimo = 0;
    fila->capacidade = 0;
}

#undef INC_MOD

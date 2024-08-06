#ifndef FILAS_H
#define FILAS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct TCelula {
    int idProcesso;
    struct TCelula* proxId;
}TCelulaFilaId;
typedef TCelulaFilaId* PCelulaFilaId;

typedef struct {
    PCelulaFilaId primeiro;
    PCelulaFilaId ultimo;
}TFilaId;
typedef TFilaId* PFilaId;

// Inicia fila com null 
bool inicializarFila(PFilaId fila); // cria a cabeça e seta idProcesso para NULL

// Funcao para verificar se a fila esta vazia ( se o prox da cabça é null)
bool estaVazia(PFilaId fila);

// Funcao para enfileirar um novo processo na fila
bool enfileirar(PFilaId fila, int idProcesso);

// Funcao para desenfileirar um processo da fila
int desenfileirar(PFilaId fila); // tira o PRIMEIRO da lista

// Funcao para liberar a memoria alocada para a fila
void ApagaFila(PFilaId fila); 

void imprimeFila(PFilaId fila);

int retiraProcessoFila(PFilaId fila, int idProcesso);

#endif
#ifndef FILAS_H
#define FILAS_H

#include <stdbool.h>

// Implementação de fila com lista encadeada

typedef struct TCelula {
    int idProcesso;
    struct TCelula* proxId;
} TCelulaFilaId;

typedef TCelulaFilaId* PCelulaFilaId;

typedef struct {
    PCelulaFilaId primeiro;
    PCelulaFilaId ultimo;
} TFilaId;

typedef TFilaId* PFilaId;

// Inicia fila vazia
bool inicializaFila(PFilaId fila);

// Funcao para verificar se a fila esta vazia ( se o prox da cabça é null)
bool estaVazia(PFilaId fila);

// Funcao para enfileirar um novo processo na fila
bool enfileirar(PFilaId fila, int idProcesso);

// Funcao para desenfileirar um processo da fila
int desenfileirar(PFilaId fila);

// Funcao para liberar a memoria alocada para a fila
void liberarFila(PFilaId fila);

void imprimeFila(PFilaId fila);

int retiraProcessoFila(PFilaId fila, int idProcesso);

#endif

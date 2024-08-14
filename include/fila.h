#ifndef FILA_H
#define FILA_H

#include <stdbool.h>
#define CAP_INICIAL 8

// Fila de IDs de processo
typedef struct {
    int *idProcessos;
    int primeiro, ultimo;
    int capacidade;
} FilaID;

// Inicializa fila
void filaInicia(FilaID *fila);

// Adiciona ID de processo à fila
void filaAdiciona(FilaID *fila, int idProcesso);

// Remove ID da fila e retorna-o
int filaRemove(FilaID *fila);

// Imprime a fila de forma legível
void filaImprime(const FilaID *fila);

// Checa se a fila está vazia
bool filaEstaVazia(const FilaID *fila);

// Desaloca a fila
void filaLibera(FilaID *fila);

#endif // FILA_H

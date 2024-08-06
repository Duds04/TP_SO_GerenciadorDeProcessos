#ifndef FILAS_MULTIPLAS_H
#define FILAS_MULTIPLAS_H

#include "filasID.h"
#include "tabela.h"
#include <stdbool.h>

// Definindo uma estrutura para armazenar as filas multiplas

typedef TfilasPrioridades* PfilasPrioridades;
typedef struct {
    int numCPUs;     // Numero de CPUs
    PFilaId vetorPrioridades[4]; // Vetor de prioridades
}TfilasPrioridades;


int inicializaFilas(PfilasPrioridades filas);

int colocaProcesso(const* tabelaProcesso, PfilasPrioridades filas ,int  idProcesso, int estadoAnterior);

#endif 
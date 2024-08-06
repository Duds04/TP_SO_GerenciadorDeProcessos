#ifndef FILAS_MULTIPLAS_H
#define FILAS_MULTIPLAS_H

#include "filasID.h"
#include "tabela.h"
#include <stdbool.h>

// Definindo uma estrutura para armazenar as filas multiplas

typedef struct {
    int numCPUs;     // Numero de CPUs
    PFilaId vetorPrioridades[4]; // Vetor de prioridades
}TfilasPrioridades;
typedef TfilasPrioridades* PfilasPrioridades;


int inicializaFilas(PfilasPrioridades filas);

// Recebe o id do processo e o estado anterior do processo (bloqueado ou execucao) e coloca o processo no escalonador. Retorna 1 se der errado
int colocaProcesso(const* tabelaProcesso, PfilasPrioridades filas ,int  idProcesso, int estadoAnterior);

// Retorna o id do processo a ser executado na vez atual. Retorna -1 se não houver processo a ser executado
int retiraProcesso(PfilasPrioridades filas);


// retorna o tamanho do quantum de acordo com a prioridade. Se a prioridade for invalida, retorna 0
int tamanhoQuantumPrioridade(int prioridade);

void imprimeFilasMultiplas(PfilasPrioridades filas);

#endif 
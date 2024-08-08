#ifndef FILAS_MULTIPLAS_H
#define FILAS_MULTIPLAS_H

#include <stdbool.h>

#include "processo.h"
#include "filasID.h"

// Estrutura para armazenar as filas múltiplas
typedef struct {
    TFilaId vetorPrioridades[4];
} TfilasPrioridades;

typedef TfilasPrioridades* PfilasPrioridades;

void inicializaTodasFilas(PfilasPrioridades filas);

// Recebe o id do processo e o estado anterior do processo (bloqueado ou
// execucao) e coloca o processo no escalonador. Retorna 1 se der errado
int enfileiraProcesso(PfilasPrioridades filas, Tprocesso *processo);

// Retorna o id do processo a ser executado na vez atual. Retorna -1 se não
// houver processo a ser executado
int retiraProcesso(PfilasPrioridades filas);

// Retorna o tamanho do quantum de acordo com a prioridade. Se a prioridade
// for inválida, retorna 0
int tamanhoQuantumPrioridade(int prioridade);

void imprimeFilasMultiplas(PfilasPrioridades filas);

#endif

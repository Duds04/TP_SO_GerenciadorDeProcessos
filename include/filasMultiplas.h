#ifndef FILAS_MULTIPLAS_H
#define FILAS_MULTIPLAS_H

#include <stdbool.h>

#include "processo.h"
#include "fila.h"

// Estrutura para armazenar as filas múltiplas
typedef struct {
    FilaID prioridade[4];
} FilasMultiplas;

void filasMultiplasInicia(FilasMultiplas *filas);

// Baseando-se nos dados do processo, adiciona seu ID à fila correta
int filasMultiplasAdiciona(FilasMultiplas *filas, Processo *processo);

// Remove o ID do próximo processo a ser executado
int filasMultiplasRemove(FilasMultiplas *filas);

// Retorna o tamanho do quantum de acordo com a prioridade. Se a prioridade
// for inválida, retorna 0
int filasMultiplasQuantum(int prioridade);

void filasMultiplasImprime(const FilasMultiplas *filas);

void filasMultiplasLibera(FilasMultiplas *filas);

#endif

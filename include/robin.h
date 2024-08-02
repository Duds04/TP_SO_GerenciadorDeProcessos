#ifndef ROBIN_H
#define ROBIN_H

#include <stdbool.h>

#define CAP_INICIAL 8

// Fila para uso no chaveamento circular (Round-Robin)
typedef struct {
    int *id_processos;
    int primeiro, ultimo;
    int capacidade;
} FilaRobin;


void robin_inicia(FilaRobin *r);

void robin_libera(FilaRobin *r);

// Retorna o id do processo escolhido
int robin_retira(FilaRobin *r);

void robin_adiciona(FilaRobin *r, int id_processo);

void robin_imprime(const FilaRobin *r);

bool robin_vazio(const FilaRobin *r);

#endif // ROBIN_H
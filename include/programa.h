#ifndef PROGRAMA_H
#define PROGRAMA_H

#include <stdio.h>
#include "instrucao.h"

// Programa = lista de instruções + número de registradores
typedef struct {
    Instrucao *intrucoes;
    int ultimo, tamanho;
    int numRegs;
} Programa;

void programaInicia(Programa *prog);

void programaAdiciona(Programa *prog, Instrucao inst);

void programaImprime(const Programa *prog, int inicio);

// Faz uma cópia profunda do programa
void programaCopia(const Programa *src, Programa *dst);

void programaCarrega(Programa *prog, FILE *arq);

void programaLibera(Programa *prog);

#endif // PROGRAMA_H

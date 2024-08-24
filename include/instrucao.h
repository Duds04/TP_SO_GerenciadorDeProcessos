#ifndef INSTRUCAO_H
#define INSTRUCAO_H

#include <stdio.h>
#define TAM_INIT 32

typedef struct {
    char op;            // código de operação: N, D, V, A, S, B, T, F ou R
    int arg0, arg1;     // argumentos da instrução, na ordem
    char arq[TAM_INIT]; // nome do arquivo a ser carregado; apenas na operação R
} Instrucao;

// Carrega uma instrução a partir da linha correspondente no código, retornando
// um valor diferente de zero em caso de instrução inválida
int instrucaoCarrega(Instrucao *inst, const char *linha);

void instrucaoImprime(const Instrucao *inst);

// Programa = lista de instruções + número de registradores
typedef struct {
    Instrucao *intrucoes;
    int ultimo, tamanho;
    int numRegs;
} Programa;

void programaInicia(Programa *prog);

void programaAdiciona(Programa *prog, Instrucao inst);

void programaImprime(const Programa *prog, int inicio);

// Faz uma cópia profunda da lista de instruções
void programaCopia(const Programa *src, Programa *dst);

// Carrega um arquivo completo como uma lista de instruções
void programaCarrega(Programa *prog, FILE *arq);

void programaLibera(Programa *prog);

#endif // INSTRUCOES_H

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
int carrega_instrucao(Instrucao *inst, const char *linha);

typedef struct {
    Instrucao *intrucoes;
    int ultimo;
    int tamanho;
} TListaInstrucao;

void liIniciaLista(TListaInstrucao *pLista);
void liInsereFinal(TListaInstrucao *pLista, Instrucao instrucao);
void liImprimeLista(TListaInstrucao* pLista);
void liImprimeInstrucao(Instrucao instrucao);
void liLiberaLista(TListaInstrucao *pLista);

// Carrega um arquivo completo como uma lista de instruções. Retorna um número
// positivo de registradores que devem ser alocados para o programa
int carrega_executavel(TListaInstrucao *prog, FILE *arq);

#endif // INSTRUCOES_H

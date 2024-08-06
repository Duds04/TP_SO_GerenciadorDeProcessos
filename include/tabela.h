#ifndef TABELA_H
#define TABELA_H

#include "instrucao.h"
#include "processo.h"

typedef struct TTabelaProcesso {
    Tprocesso *processos;
    int ultimo;
    int tamanho;
} TTabelaProcesso;

void tpIniciaLista(TTabelaProcesso *pLista);

// Adiciona um processo à tabela, retornando seu ID
int tpAdicionaProcesso(TTabelaProcesso *pLista, int id_pai, int pc,
        int prioridade, int num_regs, TListaInstrucao codigo, int tempoInicio);

// Finaliza um processo na tabela
void tpFinalizaProcesso(TTabelaProcesso *pLista, int id);

// Retorna um ponteiro para o processo dado seu ID
Tprocesso *tpAcessaProcesso(const TTabelaProcesso *pLista, int id);

void tpImprimeLista(TTabelaProcesso* pLista);

void tpLiberaLista(TTabelaProcesso *pLista);

#endif // TABELA_H

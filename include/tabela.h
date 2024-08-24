#ifndef TABELA_H
#define TABELA_H

#include <stdint.h>
#include "instrucao.h"
#include "processo.h"

typedef struct {
    Processo *processos;
    int ultimo, tamanho;
    int contadorProcessos; // conta os processos atuais em memória
    int contadorTodosProcessos; // conta quantos processos passaram pela CPU
} TabelaProcessos;

void tabelaProcessosInicia(TabelaProcessos *tab);

// Adiciona um processo à tabela, retornando seu ID. O processo herda a
// a prioridade do pai ou tem prioridade 0, caso não tenha pai
int tabelaProcessosAdiciona(TabelaProcessos *tab, int idPai, int pc,
        Programa codigo, int tempoInicio);

// Finaliza um processo na tabela
void tabelaProcessoRemove(TabelaProcessos *tab, int id);

// Retorna um ponteiro para o processo dado seu ID
Processo *tabelaProcessosAcessa(const TabelaProcessos *tab, int id);

void tabelaProcessosImprime(const TabelaProcessos *tab);

void tabelaProcessosLibera(TabelaProcessos *tab);

#endif // TABELA_H

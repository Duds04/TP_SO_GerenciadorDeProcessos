#ifndef TABELA_H
#define TABELA_H

#include "processo.h"

typedef struct TTabelaProcesso{
    Tprocesso *tabelaProcessos;
    int ultimo;
    int tamanho;
} TTabelaProcesso;

void tpIniciaLista(TTabelaProcesso *pLista);
void tpAumentaTamanhoDaLista(TTabelaProcesso *pLista);
void tpInsereFinal(TTabelaProcesso *pLista, Tprocesso processo);
void tpImprimeLista(TTabelaProcesso* pLista);
void tpImprimeInstrucao(TTabelaProcesso instrucao);
void tpLiberaLista(TTabelaProcesso *pLista);

#endif // TABELA_H

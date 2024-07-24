#include "Tprocesso.h"

typedef struct TTabelaProcesso{
    Tprocesso *tabelaProcessos;
    int ultimo;
    int tamanho;
} TTabelaProcesso;

int tpIniciaLista(TTabelaProcesso *pLista);
int tpAumentaTamanhoDaLista(TTabelaProcesso *pLista);
int tpInsereFinal(TTabelaProcesso *pLista, Tprocesso processo);
void tpImprimeLista(TTabelaProcesso* pLista);
void tpImprimeInstrucao(TTabelaProcesso instrucao);
void tpLiberaLista(TTabelaProcesso *pLista);
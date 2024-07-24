#include <stdio.h>
#include <stdlib.h>

#include "tabela.h"
#include "instrucao.h"

void tpIniciaLista(TTabelaProcesso *pLista){
    Tprocesso *listProcess = (Tprocesso *) malloc(TAM_INIT * sizeof(Tprocesso));

    if (listProcess == NULL) {
        printf("Erro ao alocar memória!");
        exit(1);
    }

    pLista->tabelaProcessos = listProcess;
    pLista->ultimo = 0;
}

void tpAumentaTamanhoDaLista(TTabelaProcesso *pLista){

    pLista->tabelaProcessos = realloc(pLista->tabelaProcessos, pLista->tamanho*2 * sizeof(Tprocesso));

    if (pLista->tabelaProcessos == NULL) {
        printf("Erro ao realocar memória!");
        exit(1);
    }
    pLista->tamanho = pLista->tamanho*2;
}

void tpInsereFinal(TTabelaProcesso *pLista, Tprocesso processo){
    if(pLista->ultimo == pLista->tamanho){
        tpAumentaTamanhoDaLista(pLista);
    }
    pLista->tabelaProcessos[pLista->ultimo] = processo;
    pLista->ultimo++;
}

void tpImprimeLista(TTabelaProcesso *pLista){
    for(int i=0; i<pLista->ultimo; i++){
        //liImprimeInstrucao(pLista->listaIntrucao[i]);
    }
}


void tpLiberaLista(TTabelaProcesso *pLista){
    for(int i=0; i<pLista->ultimo;i++){
        liLiberaLista(&pLista->tabelaProcessos[i].listaInstrucao);
    }
    free(pLista->tabelaProcessos);
}

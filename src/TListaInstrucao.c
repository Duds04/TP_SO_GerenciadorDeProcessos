#include "TListaInstrucao.h"

int LIIniciaLista(TListaInstrucao *pLista){
    Tinstrucao *listInstruct;

    listInstruct = (Tinstrucao *) malloc(TAM_INIT * sizeof(Tinstrucao));

    if (listInstruct == NULL) {
        printf("Erro ao alocar memória!");
        return 1;
    }

    pLista->listaIntrucao = listInstruct;
    pLista->primeiro = 0;
    pLista->ultimo = pLista->primeiro;

    return 0;
}

int LIAumentaTamanhoDaLista(TListaInstrucao *pLista){

    pLista->listaIntrucao = realloc(pLista->listaIntrucao, pLista->tamanho*2 * sizeof(int));
    
    if (pLista->listaIntrucao == NULL) {
        printf("Erro ao realocar memória!");
        return 1;
    }
    pLista->tamanho = pLista->tamanho*2;

    return 0;
}
int LIInsereFinal(TListaInstrucao *pLista, Tinstrucao *instrucao, int linha){

}
int LINumeroPalavras(TListaInstrucao *pLista){

}
void LIImprimeLista(TListaInstrucao* pLista){

}
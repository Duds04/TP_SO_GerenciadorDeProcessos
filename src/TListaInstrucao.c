#include "TListaInstrucao.h"

int liIniciaLista(TListaInstrucao *pLista){
    Tinstrucao *listInstruct;

    listInstruct = (Tinstrucao *) malloc(TAM_INIT * sizeof(Tinstrucao));

    if (listInstruct == NULL) {
        printf("Erro ao alocar memória!");
        exit;
    }

    pLista->listaIntrucao = listInstruct;
    pLista->ultimo = 0;

    return 0;
}

int liAumentaTamanhoDaLista(TListaInstrucao *pLista){

    pLista->listaIntrucao = realloc(pLista->listaIntrucao, pLista->tamanho*2 * sizeof(Tinstrucao));
    
    if (pLista->listaIntrucao == NULL) {
        printf("Erro ao realocar memória!");
        exit;
    }
    pLista->tamanho = pLista->tamanho*2;

    return 0;
}

int liInsereFinal(TListaInstrucao *pLista, Tinstrucao instrucao){
    if(pLista->ultimo == pLista->tamanho){
        liAumentaTamanhoDaLista(&pLista);
    }
    pLista->listaIntrucao[pLista->ultimo] = instrucao;
    pLista->ultimo++;
    
}

void liImprimeLista(TListaInstrucao *pLista){
    for(int i=0; i<pLista->ultimo; i++){
        liImprimeInstrucao(pLista->listaIntrucao[i]);
    }
}

void liImprimeInstrucao(Tinstrucao instrucao){
    switch (instrucao.tipoIntrucao){
    case 'N':
    case 'D':
    case 'B':
    case 'F':
        printf("%c %d\n", instrucao.tipoIntrucao, instrucao.numeroEntrada1);
        break;
    case 'V':
    case 'A':
    case 'S':
        printf("%c %d %d\n", instrucao.tipoIntrucao, instrucao.numeroEntrada1, instrucao.numeroEntrada2);
        break;
    case 'T':
        printf("%c\n", instrucao.tipoIntrucao);
        break;
    case 'R':
        printf("%c %s\n", instrucao.tipoIntrucao, instrucao.nomeArquivo);
        break;
    
    }
    
}

void liLiberaLista(TListaInstrucao *pLista){
    free(pLista->listaIntrucao);
}
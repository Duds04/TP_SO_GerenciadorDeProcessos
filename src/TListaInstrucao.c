#include "TListaInstrucao.h"

int liIniciaLista(TListaInstrucao *pLista){
    Tinstrucao *listInstruct;

    listInstruct = (Tinstrucao *) malloc(TAM_INIT * sizeof(Tinstrucao));

    if (listInstruct == NULL) {
        printf("Erro ao alocar memória!");
        exit(1);
    }

    pLista->intrucoes = listInstruct;
    pLista->ultimo = 0;

    return 0;
}

int liAumentaTamanhoDaLista(TListaInstrucao *pLista){

    pLista->intrucoes = realloc(pLista->intrucoes, pLista->tamanho*2 * sizeof(Tinstrucao));
    
    if (pLista->intrucoes == NULL) {
        printf("Erro ao realocar memória!");
        exit(1);
    }
    pLista->tamanho = pLista->tamanho*2;

    return 0;
}

int liInsereFinal(TListaInstrucao *pLista, Tinstrucao instrucao){
    if(pLista->ultimo == pLista->tamanho){
        liAumentaTamanhoDaLista(&pLista);
    }
    pLista->intrucoes[pLista->ultimo] = instrucao;
    pLista->ultimo++;
    
}

void liImprimeLista(TListaInstrucao *pLista){
    for(int i=0; i<pLista->ultimo; i++){
        liImprimeInstrucao(pLista->intrucoes[i]);
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
    free(pLista->intrucoes);
}
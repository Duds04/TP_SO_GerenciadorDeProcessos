#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#define TAM_INIT 30

typedef struct Tinstrucao{
    char tipoIntrucao;
    int numeroEntrada1;
    int numeroEntrada2;
    char nomeArquivo[30];
}Tinstrucao;

typedef struct{
    Tinstrucao *listaIntrucao;
    int primeiro, ultimo;
    int tamanho;
} TListaInstrucao;

int LIIniciaLista(TListaInstrucao *pLista);
int LIAumentaTamanhoDaLista(TListaInstrucao *pLista);
int LIInsereFinal(TListaInstrucao *pLista, Tinstrucao *instrucao, int linha);
int LINumeroPalavras(TListaInstrucao *pLista);
//int LPVerificaPalavra(TListaInstrucao *pLista, Tinstrucao *instrucao, int linha);
void LIImprimeLista(TListaInstrucao* pLista);
//int RemovePalavraFinal(TListaInstrucao *pLista);
//int RemovePalavraDada(TListaInstrucao *pLista, char *palavra);
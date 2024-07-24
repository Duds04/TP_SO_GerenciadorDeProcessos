#include <stdio.h>
#include <unistd.h>
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
} TListaInstrucao;

int LPIniciaLista(TListaInstrucao *pLista);
int LPInsereFinal(TListaInstrucao *pLista, Tinstrucao *instrucao, int linha);
int LPNumeroPalavras(TListaInstrucao *pLista);
//int LPVerificaPalavra(TListaInstrucao *pLista, Tinstrucao *instrucao, int linha);
void LImprimeListaPalavra(TListaInstrucao* pLista);
//int RemovePalavraFinal(TListaInstrucao *pLista);
//int RemovePalavraDada(TListaInstrucao *pLista, char *palavra);
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define TAM_INIT 32

typedef struct Tinstrucao{
    char tipoIntrucao;
    int numeroEntrada1;
    int numeroEntrada2;
    char nomeArquivo[TAM_INIT];
}Tinstrucao;

typedef struct TListaInstrucao{
    Tinstrucao *listaIntrucao;
    int ultimo;
    int tamanho;
} TListaInstrucao;

int liIniciaLista(TListaInstrucao *pLista);
int liAumentaTamanhoDaLista(TListaInstrucao *pLista);
int liInsereFinal(TListaInstrucao *pLista, Tinstrucao instrucao);
void liImprimeLista(TListaInstrucao* pLista);
void liImprimeInstrucao(Tinstrucao instrucao);
void liLiberaLista(TListaInstrucao *pLista);
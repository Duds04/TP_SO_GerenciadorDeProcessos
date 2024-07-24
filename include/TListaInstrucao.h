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

typedef struct TListaInstrucao{
    Tinstrucao *listaIntrucao;
    int ultimo;
    int tamanho;
} TListaInstrucao;

int liIniciaLista(TListaInstrucao *pLista);
int liAumentaTamanhoDaLista(TListaInstrucao *pLista);
int liInsereFinal(TListaInstrucao *pLista, Tinstrucao instrucao);
void liImprimeInstrucao(TListaInstrucao* pLista);
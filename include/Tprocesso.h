#include "TListaInstrucao.h"

typedef struct TADprocesso{
    int idProcesso;
    int idProcessoPai;
    int PC; // usado de índice para a lista de instruções
    //estrutura de dados
    //prioridade
    //estado
    TListaInstrucao *listaInstrucao;
    int tempoInicio;
    int tempoCPU;
}processo;
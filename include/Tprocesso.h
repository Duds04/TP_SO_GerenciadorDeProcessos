#include "TListaInstrucao.h"

typedef struct TADprocesso{
    int idProcesso;
    int idProcessoPai;
    int PC;
    //estrutura de dados
    //prioridade
    //estado
    TListaInstrucao *listaInstrucao;
    int tempoInicio;
    int tempoCPU;
}processo;
#include "TListaInstrucao.h"

typedef struct Tprocesso{
    int idProcesso;
    int idProcessoPai;
    int pc; // usado de índice para a lista de instruções
    //estrutura de dados
    //prioridade
    //estado
    TListaInstrucao listaInstrucao;
    int tempoInicio;
    int tempoCPU;
}Tprocesso;
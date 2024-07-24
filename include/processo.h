#ifndef PROCESSO_H
#define PROCESSO_H

#include "instrucao.h"

typedef struct {
    int idProcesso;
    int idProcessoPai;
    int pc; // usado de índice para a lista de instruções
    //estrutura de dados
    //prioridade
    //estado
    TListaInstrucao listaInstrucao;
    int tempoInicio;
    int tempoCPU;
} Tprocesso;

#endif // PROCESSO_H

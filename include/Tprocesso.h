#include <stdio.h>
#include <unistd.h>

typedef struct TADprocesso{
    int idProcesso;
    int idProcessoPai;
    int PC;
    //estrutura de dados
    //prioridade
    //estado
    int tempoInicio;
    int tempoCPU;
}processo;
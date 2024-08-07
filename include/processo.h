#ifndef PROCESSO_H
#define PROCESSO_H

#include "instrucao.h"

// Estados em que um processo pode estar
typedef enum {
    EST_PRONTO,
    EST_EXECUTANDO,
    EST_BLOQUEADO,
    EST_FINALIZADO, // estado sentinela para processos mortos (temporários)
} ProcessoEstado;

// Informações de um processo na tabela
typedef struct {
    int id, id_pai;         // ID do processo e do processo pai
    int pc;                 // último valor do contador de programa (tempo de CPU)
    int prioridade;         // prioridade do processo
    int *reg, num_regs;     // registradores do processo
    ProcessoEstado estado;  // estado do processo
    TListaInstrucao codigo; // instruções do processo
    int tempoInicio;        // unidade de tempo de ínicio
} Tprocesso;


// Inicializa processo
void processo_inicia(Tprocesso *proc, int id, int id_pai, int pc, int prioridade,
        int num_regs, TListaInstrucao codigo, int tempoInicio);

void processo_dados(const Tprocesso *proc);

// Imprime os dados de um processo
void processo_imprime(const Tprocesso *proc);

// Desaloca a memória associada a um processo
void processo_libera(Tprocesso *proc);

// Substituir código de um processo]
void substituiPrograma(Tprocesso *proc, TListaInstrucao codigo, int num_regs);

#endif // PROCESSO_H

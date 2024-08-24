#ifndef ESCALONAMENTO_H
#define ESCALONAMENTO_H

// Interface geral dos algoritmos de escalonamento. Cada uma das funções aqui é
// um "dispatcher", que seleciona a função apropriada de escalonamento com base
// na identificação do algoritmo

#include "processo.h"

// Identificação de cada algoritmo de escalonamento
typedef enum {
    ESC_ROBIN = 0,
    ESC_FILAS_MULTIPLAS = 1,
    NUM_ESCALONAMENTOS,
} EscalonamentoID;

// Escalonador genérico
typedef struct {
    EscalonamentoID id;
    void *dados;
} Escalonador;

// Inicializa um escalonador
void escalonadorInicia(Escalonador *esc, EscalonamentoID escId);

// Adiciona um processo ao escalonador
int escalonadorAdiciona(Escalonador esc, Processo *proc);

// Retira um processo do escalonador, retornando seu ID
int escalonadorRemove(Escalonador esc);

// Fornece o tamanho do quantum, em unidades de tempo, correspondente ao número
// de prioridade dado
int escalonadorQuantum(Escalonador esc, int prioridade);

void escalonadorImprime(Escalonador esc);

// Desaloca a memória asssociada a um escalonador
void escalonadorLibera(Escalonador esc);

#endif // ESCALONAMENTO_H

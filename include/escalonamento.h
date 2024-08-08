#ifndef ESCALONAMENTO_H
#define ESCALONAMENTO_H

// Interface geral dos algoritmos de escalonamento. Cada uma das funções aqui é
// um "dispatcher", que seleciona a função apropriada de escalonamento com base
// na identificação do algoritmo

#include "processo.h"

// Identificação de cada algoritmo de escalonamento
typedef enum : int {
    ESC_ROBIN = 0,
    ESC_FILAS_MULTIPLAS = 1,
    NUM_ESCALONAMENTOS,
} Escalonamento;

// Adiciona um processo ao escalonador
int esc_adiciona_processo(Escalonamento esc, void *escalonador, Tprocesso *proc);

// Retira um processo do escalonador, retornando seu ID
int esc_retira_processo(Escalonamento esc, void *escalonador);

// Fornece o tamanho do quantum, em unidades de tempo, correspondente ao número
// de prioridade dado
int esc_tamanho_quantum(Escalonamento esc, int prioridade);

#endif // ESCALONAMENTO_H

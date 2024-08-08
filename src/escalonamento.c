#include <stdio.h>

#include "robin.h"
#include "escalonamento.h"
#include "filasMultiplas.h"

// Adiciona um processo ao escalonador
int esc_adiciona_processo(Escalonamento esc, void *escalonador, Tprocesso *proc) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return enfileiraProcesso(escalonador, proc);
        case ESC_ROBIN:
            robin_adiciona(escalonador, proc->id);
            return 0;
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc);
            return -1;
    }
}

// Retira um processo do escalonador, retornando seu ID
int esc_retira_processo(Escalonamento esc, void *escalonador) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return retiraProcesso(escalonador);
        case ESC_ROBIN:
            return robin_retira(escalonador);
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc);
            return -1;
    }
}

// Fornece o tamanho do quantum, em unidades de tempo, correspondente ao número
// de prioridade dado
int esc_tamanho_quantum(Escalonamento esc, int prioridade) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return tamanhoQuantumPrioridade(prioridade);
        case ESC_ROBIN:
            // O escalonamento round robin não trabalha com o conceito de
            // prioridade; o quantum é fixo
            return 3;
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc);
            return 1;
    }
}

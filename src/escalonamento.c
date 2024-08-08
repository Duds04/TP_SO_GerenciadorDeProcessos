#include "escalonamento.h"
#include "filasMultiplas.h"
#include <stdio.h>

// Adiciona um processo ao escalonador
int esc_adiciona_processo(Escalonamento esc, void *escalonador, Tprocesso *proc) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return enfileiraProcesso((PfilasPrioridades) escalonador, proc);
        default:
            fprintf(stderr, "[!] Escalonamento %d não integrado\n", esc);
            return 1;
    }
}

// Retira um processo do escalonador, retornando seu ID
int esc_retira_processo(Escalonamento esc, void *escalonador) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return retiraProcesso((PfilasPrioridades) escalonador);
        default:
            fprintf(stderr, "[!] Escalonamento %d não integrado\n", esc);
            return -1;
    }
}

// Fornece o tamanho do quantum, em unidades de tempo, correspondente ao número
// de prioridade dado
int esc_tamanho_quantum(Escalonamento esc, int prioridade) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return tamanhoQuantumPrioridade(prioridade);
        default:
            fprintf(stderr, "[!] Escalonamento %d não integrado\n", esc);
            return 1;
    }
}

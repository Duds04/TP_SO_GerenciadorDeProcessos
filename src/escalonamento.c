#include <stdio.h>

#include "fila.h"
#include "escalonamento.h"
#include "filasMultiplas.h"

// Adiciona um processo ao escalonador
int escalonamentoAdiciona(Escalonamento esc, void *escalonador, Processo *proc) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return filasMultiplasAdiciona(escalonador, proc);
        case ESC_ROBIN:
            filaAdiciona(escalonador, proc->id);
            return 0;
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc);
            return -1;
    }
}

// Retira um processo do escalonador, retornando seu ID
int escalonamentoRemove(Escalonamento esc, void *escalonador) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return filasMultiplasRemove(escalonador);
        case ESC_ROBIN:
            return filaRemove(escalonador);
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc);
            return -1;
    }
}

// Fornece o tamanho do quantum, em unidades de tempo, correspondente ao número
// de prioridade dado
int escalonamentoQuantum(Escalonamento esc, int prioridade) {
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return filasMultiplasQuantum(prioridade);
        case ESC_ROBIN:
            // O escalonamento round robin não trabalha com o conceito de
            // prioridade; o quantum é fixo
            return 3;
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc);
            return 1;
    }
}

void escalonamentoImprime(Escalonamento esc, void *escalonador){
    switch(esc) {
        case ESC_FILAS_MULTIPLAS:
            return filasMultiplasImprime(escalonador);
        case ESC_ROBIN:
            return filaImprime(escalonador);
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc);
    }
}

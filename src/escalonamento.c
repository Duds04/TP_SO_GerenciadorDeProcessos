#include <stdio.h>
#include <stdlib.h>

#include "fila.h"
#include "escalonamento.h"
#include "filasMultiplas.h"

// Inicializa um escalonador
void escalonadorInicia(Escalonador *esc, EscalonamentoID escId) {
    void *dados;
    esc->id = escId;
    switch(escId) {
        case ESC_FILAS_MULTIPLAS:
            dados = malloc(sizeof(FilasMultiplas));
            if(dados == NULL) exit(64);
            filasMultiplasInicia(dados);
            break;
        case ESC_ROBIN:
            dados = malloc(sizeof(FilaID));
            if(dados == NULL) exit(64);
            filaInicia(dados);
            break;
        default:
            exit(65);
    }
    esc->dados = dados;
}

// Adiciona um processo ao escalonador
int escalonadorAdiciona(Escalonador esc, Processo *proc) {
    switch(esc.id) {
        case ESC_FILAS_MULTIPLAS:
            return filasMultiplasAdiciona(esc.dados, proc);
        case ESC_ROBIN:
            filaAdiciona(esc.dados, proc->id);
            return 0;
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc.id);
            return -1;
    }
}

// Retira um processo do escalonador, retornando seu ID
int escalonadorRemove(Escalonador esc) {
    switch(esc.id) {
        case ESC_FILAS_MULTIPLAS:
            return filasMultiplasRemove(esc.dados);
        case ESC_ROBIN:
            return filaRemove(esc.dados);
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc.id);
            return -1;
    }
}

// Fornece o tamanho do quantum, em unidades de tempo, correspondente ao número
// de prioridade dado
int escalonadorQuantum(Escalonador esc, int prioridade) {
    switch(esc.id) {
        case ESC_FILAS_MULTIPLAS:
            return filasMultiplasQuantum(prioridade);
        case ESC_ROBIN:
            // O escalonamento round robin não trabalha com o conceito de
            // prioridade; o quantum é fixo
            return 3;
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc.id);
            return 1;
    }
}

void escalonadorImprime(Escalonador esc){
    switch(esc.id) {
        case ESC_FILAS_MULTIPLAS:
            return filasMultiplasImprime(esc.dados);
        case ESC_ROBIN:
            return filaImprime(esc.dados);
        default:
            fprintf(stderr, "[!] Escalonamento %d não reconhecido\n", esc.id);
    }
}

// Desaloca a memória asssociada a um escalonador
void escalonadorLibera(Escalonador esc) {
    switch(esc.id) {
        case ESC_FILAS_MULTIPLAS:
            filasMultiplasLibera(esc.dados);
            break;
        case ESC_ROBIN:
            filaLibera(esc.dados);
            break;
        default:
            break;
    }
    free(esc.dados);
    esc.id = -1;
}

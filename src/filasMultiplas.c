#include <stdio.h>
#include <stdbool.h>

#include "fila.h"
#include "filasMultiplas.h"
#include "processo.h"

void filasMultiplasInicia(FilasMultiplas *filas){
    for (int i = 0; i < 4; i++)
        filaInicia(&filas->prioridade[i]);
}

// Baseando-se nos dados do processo, adiciona seu ID à fila correta
int filasMultiplasAdiciona(FilasMultiplas *filas, Processo *processo) {
    int prioridade = processo->prioridade, id = processo->id, i;
    // se o estado anterior era pronto, a sua prioridade diminui
    // imprimeFilasMultiplas(filas);
    switch(processo->estado) {
        case EST_PRONTO:
        case EST_EXECUTANDO:
            // ID da fila depende da prioridade; 3 é a máxima
            i = (prioridade < 3) ? prioridade + 1 : 3;
            filaAdiciona(&filas->prioridade[i], id);
            processo->prioridade = i;
            processo->estado = EST_PRONTO;
            return 0;
        case EST_BLOQUEADO:
            // ID da fila depende da prioridade; 3 é a máxima
            i = (prioridade > 0) ? prioridade - 1 : 0;
            filaAdiciona(&filas->prioridade[i], id);
            processo->prioridade = i;
            processo->estado = EST_PRONTO;
            return 0;
        default:
            fprintf(stderr, "[!] Estado anterior inválido (filasMultiplas)\n");
            return 1;
    }
}

// Remove o ID do próximo processo a ser executado
int filasMultiplasRemove(FilasMultiplas *filas){
    for (int i = 0; i < 4; i++){
        if (filaEstaVazia(&filas->prioridade[i]))
            continue; // avança para a próxima fila
        return filaRemove(&filas->prioridade[i]);
    }
    return -1;
}

// Retorna o tamanho do quantum de acordo com a prioridade. Se a prioridade
// for inválida, retorna 0
int filasMultiplasQuantum(int prioridade){
    switch (prioridade){
        case 0: return 1;
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        default:
            fprintf(stderr, "[!] Prioridade inválida (filasMultiplas)\n");
            return 0;
    }
}

void filasMultiplasImprime(const FilasMultiplas *filas){
    printf("Filas multiplas: \n");
    for (int i = 0; i < 4; i++){
        printf("Fila de prioridade %d: \n", i);
        filaImprime(&filas->prioridade[i]);
    }
}

void filasMultiplasLibera(FilasMultiplas *filas) {
    for(int i = 0; i < 4; ++i)
        filaLibera(&filas->prioridade[i]);
}

#include <stdio.h>
#include <stdbool.h>

#include "filasMultiplas.h"
#include "processo.h"

void inicializaTodasFilas(PfilasPrioridades filas){
    for (int i = 0; i < 4; i++)
        inicializaFila(&filas->vetorPrioridades[i]);
}

// Recebe o id do processo e o estado anterior do processo (bloqueado ou
// execucao) e coloca o processo no escalonador. Retorna 1 se der errado
int enfileiraProcesso(PfilasPrioridades filas, int id, Tprocesso *processo) {
    int prioridade = processo->prioridade, i;
    // se o estado anterior era pronto, a sua prioridade diminui
    // imprimeFilasMultiplas(filas);
    switch(processo->estado) {
        case EST_PRONTO:
        case EST_EXECUTANDO:
            // ID da fila depende da prioridade; 3 é a máxima
            i = (prioridade < 3) ? prioridade + 1 : 3;
            if (!enfileirar(&filas->vetorPrioridades[i], id)) {
                fprintf(stderr, "> EM FILAS MULTIPLAS: Nao foi possivel enfileirar\n");
                return 1;
            }
            processo->prioridade = i;
            processo->estado = EST_PRONTO;
            return 0;
        case EST_BLOQUEADO:
            // ID da fila depende da prioridade; 3 é a máxima
            i = (prioridade > 0) ? prioridade - 1 : 0;
            if (!enfileirar(&filas->vetorPrioridades[i], id)) {
                printf("> EM FILAS MULTIPLAS: Nao foi possivel enfileirar\n");
                return 1;
            }
            processo->prioridade = i;
            processo->estado = EST_PRONTO;
            return 0;
        default:
            printf("> EM FILAS MULTIPLAS: Estado anterior invalido\n");
            return 1;
    }
}

// Retorna o id do processo a ser executado na vez atual. Retorna -1 se não
// houver processo a ser executado
int retiraProcesso(PfilasPrioridades filas){
    for (int i = 0; i < 4; i++){
        if (estaVazia(&filas->vetorPrioridades[i]))
            continue; // avança para a próxima fila
        return desenfileirar(&filas->vetorPrioridades[i]);
    }
    return -1;
}

// Retorna o tamanho do quantum de acordo com a prioridade. Se a prioridade
// for inválida, retorna 0
int tamanhoQuantumPrioridade(int prioridade){
    switch (prioridade){
        case 0: return 1;
        case 1: return 2;
        case 2: return 4;
        case 3: return 8;
        default:
            printf("> EM FILAS MULTIPLAS: Prioridade invalida\n");
            return 0;
    }
}

void imprimeFilasMultiplas(PfilasPrioridades filas){
    fprintf(stderr, "Filas multiplas: \n");
    for (int i = 0; i < 4; i++){
        fprintf(stderr, "Fila de prioridade %d: \n", i);
        imprimeFila(&filas->vetorPrioridades[i]);
    }
}

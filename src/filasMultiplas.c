#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "filasMultiplas.h"
#include "processo.h"



int inicializaFilas(PfilasPrioridades filas){
    for (int i = 0; i < 4; i++){
        filas->vetorPrioridades[i] = NULL;
    }
    for (int i = 0; i < 4; i++){
        filas->vetorPrioridades[i] = inicializaFilas(filas->vetorPrioridades[i]);
        if (filas->vetorPrioridades[i] == NULL){
            for (int j = 0; j < i; j++){
                filas->vetorPrioridades[j] == NULL;
            }
            return 0;
        }
    }
}

// Recebe o id do processo e o estado anterior do processo (bloqueado ou execucao) e coloca o processo no escalonador. Retorna 1 se der errado
int colocaProcesso(const* tabelaProcesso, PfilasPrioridades filas ,int  idProcesso, int estadoAnterior){
    Tprocesso* processo = NULL;
    processo = tpAcessaProcesso(tabelaProcesso, idProcesso);
    if (processo == NULL){
        printf("> EM FILAS MULTIPLAS: Processo nao encontrado\n");
        return 1;
    } 
    int prioridade = processo->prioridade;
    // se o estado anterior era pronto, a sua prioridade diminui
    if (estadoAnterior == EST_PRONTO){

        if (prioridade < 3){ // Se a prioridade for 3, o processo já está na fila de prioridade máxima
            if (!enfileirar(filas->vetorPrioridades[prioridade++], idProcesso)) {
                printf("> EM FILAS MULTIPLAS: Nao foi possivel enfileirar\n");
                return 1;
            }
            processo->prioridade++;
            return 0;
        }
        else {
            if (!enfileirar(filas->vetorPrioridades[3], idProcesso)){
                printf("> EM FILAS MULTIPLAS: Nao foi possivel enfileirar\n");
                return 1;
            }
        }

    } 
    else if (estadoAnterior == EST_BLOQUEADO){
        if (prioridade > 0){ // Se a prioridade for 3, o processo já está na fila de prioridade máxima
            if (!enfileirar(filas->vetorPrioridades[prioridade--], idProcesso)) {
                printf("> EM FILAS MULTIPLAS: Nao foi possivel enfileirar\n");
                return 1;
            }
            processo->prioridade--;
            return 0;
        }
        else {
            if (!enfileirar(filas->vetorPrioridades[0], idProcesso)){
                printf("> EM FILAS MULTIPLAS: Nao foi possivel enfileirar\n");
                return 1;
            }
        }
    }
    else {
        printf("> EM FILAS MULTIPLAS: Estado anterior invalido\n");
        return 1;
    }
    return 0;

}
// Retorna o id do processo a ser executado na vez atual. Retorna -1 se não houver processo a ser executado
int retiraProcesso(PfilasPrioridades filas){
    for (int i = 0; i < 4; i++){
        if (!estaVazia(filas->vetorPrioridades[i])) continue; // Avanca para a proxima lista
        else {
            return desenfileirar(filas->vetorPrioridades[i]);
        }
    }
    return -1;
}

// retorna o tamanho do quantum de acordo com a prioridade. Se a prioridade for invalida, retorna 0
int tamanhoQuantumPrioridade(int prioridade){
    switch (prioridade){
        case 0:
            return 1;
        case 1:
            return 2;
        case 2:
            return 4;
        case 3:
            return 8;
        default:
            printf("> EM FILAS MULTIPLAS: Prioridade invalida\n");
            return 0;
    }
}
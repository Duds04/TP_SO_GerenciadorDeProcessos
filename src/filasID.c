#include "filasID.h"

// Funcao para inicializar a fila
void inicializarFila(PFilaId fila) {
    fila->proxId = (PFilaId) malloc(sizeof(TFilaId));
    fila->
    fila->proxId->proxId = NULL;
    fila->ultimoId = fila->proxId;
}

// Funcao para verificar se a fila esta vazia
bool estaVazia(PFilaId fila) {
    return (fila->frente->proxId == NULL);
}

// Funcao para enfileirar um novo processo na fila
bool enfileirar(PFilaId fila, int idProcesso){
    PFilaId novoId = (PFilaId) malloc(sizeof(TFilaId));
    if (novoId == NULL){
        return false;
    }
    fila->ultimoId->proxId = novoId;
    return true;
}

// Funcao para desenfileirar um processo da fila
bool desenfileirar(PFilaId fila, int idProcesso){
    if (*fila == NULL){
        return false;
    }
    if 
    PFilaId aux = *fila;
    *fila = (*fila)->proxId;
    free(aux);
    return true;
}

// Funcao para liberar a memoria alocada para a fila
void liberarFila(PFilaId fila);
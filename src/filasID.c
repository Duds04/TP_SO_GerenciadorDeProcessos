#include "../include/filasID.h"

// Funcao para inicializar a fila
bool inicializarFila(PFilaId fila) {
    fila->primeiro = (PCelulaFilaId) malloc(sizeof(TCelulaFilaId));
    if (fila->primeiro == NULL){
        return false;
    }
    fila->ultimo = fila->primeiro;
    fila->primeiro->proxId = NULL;
    fila->primeiro->idProcesso = -1;  
    printf("Fila inicializada com sucesso!\n");
}

// Funcao para verificar se a fila esta vazia
bool estaVazia(PFilaId fila) {
    return (fila->primeiro->proxId == NULL);
}

// Funcao para enfileirar um novo processo na fila
bool enfileirar(PFilaId fila, int idProcesso){
    PCelulaFilaId novo = (PCelulaFilaId) malloc(sizeof(TCelulaFilaId));
    novo->idProcesso = idProcesso;
    novo->proxId = NULL;
    if (novo == NULL){
        return false;
    }
    fila->ultimo->proxId = novo;
    fila->ultimo = novo;
    printf("Processo %d enfileirado com sucesso!\n", idProcesso);
    return true;
}

// Funcao para desenfileirar um processo da fila
bool desenfileirar(PFilaId fila, int idProcesso){
    if (estaVazia(fila)){
        return false;
    }
    PCelulaFilaId aux = fila->primeiro;
    fila->primeiro = fila->primeiro->proxId;
    free(aux);
    return true;
}

// Funcao para liberar a memoria alocada para a fila
void liberarFila(PFilaId fila){
    return 0;
}

void imprimeFila(PFilaId fila){
    PCelulaFilaId aux;
        if (fila == NULL || fila->primeiro == NULL) {
        printf("Fila is not properly initialized.\n");
        return;
    }
    aux = fila->primeiro->proxId;
    while (aux != NULL)
    {
        printf("%i", aux->idProcesso);
        aux = aux->proxId;
    }
    
}

int retiraProcessoFila(PFilaId fila, int idProcesso){
    PCelulaFilaId aux;
    PCelulaFilaId aux2;
    if (fila == NULL || fila->primeiro == NULL) {
        printf("Fila is not properly initialized.\n");
        return -1;
    }
    aux = fila->primeiro;
    while (aux->proxId != NULL)
    {
        if (aux->proxId->idProcesso == idProcesso){
            aux2 = aux->proxId;
            aux->proxId = aux->proxId->proxId;
            free(aux2);
            return 0;
        }
        aux = aux->proxId;
    }
    return -1;
}
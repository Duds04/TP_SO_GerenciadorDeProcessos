#include "filasID.h"

// Funcao para inicializar a fila
bool inicializaFila(PFilaId fila) {
    fila->primeiro = (PCelulaFilaId) malloc(sizeof(TCelulaFilaId));

    if (fila->primeiro == NULL){
        printf("> EM FILAS ID: Erro ao alocar memoria para celula cabeca na inicializaçao\n");
        return false;
    }

    fila->ultimo = fila->primeiro;
    fila->primeiro->proxId = NULL;
    fila->primeiro->idProcesso = -1;  
    // printf("Fila inicializada com sucesso!\n");
    return true;
}

// Funcao para verificar se a fila esta vazia
bool estaVazia(PFilaId fila) {
    return (fila->primeiro->proxId == NULL);
}

// Funcao para enfileirar um novo processo na fila
bool enfileirar(PFilaId fila, int idProcesso){
    PCelulaFilaId novo = (PCelulaFilaId) malloc(sizeof(TCelulaFilaId));
    
    if (novo == NULL){
        printf("> EM FILAS ID: Erro ao alocar memoria para nova celula\n");
        return false;
    }

    novo->idProcesso = idProcesso;
    novo->proxId = NULL;

    fila->ultimo->proxId = novo;
    fila->ultimo = novo;
    
    return true;
}

// Funcao para desenfileirar um processo da fila. retorna -1 se a fila estiver vazia
int desenfileirar(PFilaId fila){
    int idProcesso;

    if (estaVazia(fila)){
        printf("> EM FILAS ID: Fila vazia\n");
        return -1;
    }
    
    PCelulaFilaId aux = fila->primeiro->proxId;
    idProcesso = aux->idProcesso;
    fila->primeiro->proxId = aux->proxId;

    if (fila->primeiro->proxId == NULL) {
        fila->ultimo = fila->primeiro;
    }

    free(aux);
    return idProcesso;
}

// Funcao para liberar a memoria alocada para a fila
void liberarFila(PFilaId fila){
    if (!estaVazia(fila)){
        PCelulaFilaId aux = fila->primeiro->proxId;
        PCelulaFilaId auxFree = NULL;
        printf("> EM FILAS ID: Liberando memoria alocada para fila\n");
        while (aux != NULL)
        {
            auxFree = aux;
            aux = aux->proxId;
            free(auxFree);
        } 
        free(aux);
    }
    free(fila->primeiro);
    fila = NULL;
    printf("> EM FILAS ID: Fila liberada com sucesso\n");
    return 0;
}

void imprimeFila(PFilaId fila){
    PCelulaFilaId aux;
        if (fila == NULL || fila->primeiro == NULL) {
        printf("> EM FILAS ID: Fila nao inicializada corretamente.\n");
        return;
    }
    
    aux = fila->primeiro->proxId;
    while (aux != NULL)
    {   
        // fprintf(stderr, "TESTE");
        fprintf(stderr, "%i ", aux->idProcesso);
        aux = aux->proxId;
    }
    fprintf(stderr, "\n");
    
}

int retiraProcessoFila(PFilaId fila, int idProcesso){
    PCelulaFilaId aux;
    PCelulaFilaId aux2;
    if (fila == NULL || fila->primeiro == NULL) {
        printf("> EM FILA ID: fila nao foi iniciaizada corretamente \n");
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
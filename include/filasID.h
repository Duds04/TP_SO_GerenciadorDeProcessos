#ifdef FILASID_H
#endif FILASID_H

#include <stdbool.h>
#include <stdio.h>

typedef TCelulaFilaId* PCelulaFilaId;
typedef struct {
    int idProcesso;
    
    PCelulaFilaId proxId;
}TCelulaFilaId;

typedef TFilaId* PFilaId;
typedef struct {
    PCelulaFilaId primeiro;
    PCelulaFilaId ultimo;
}TFilaId;

// Inicia fila com null 
void inicializarFila(PFilaId fila); // cria a cabeça e seta idProcesso para NULL

// Funcao para verificar se a fila esta vazia ( se o prox da cabça é null)
bool estaVazia(PFilaId fila);

// Funcao para enfileirar um novo processo na fila
bool enfileirar(PFilaId fila, int idProcesso);

// Funcao para desenfileirar um processo da fila
bool desenfileirar(PFilaId fila, int idProcesso); // tira o PRIMEIRO da lista

// Funcao para liberar a memoria alocada para a fila
void ApagaFila(PFilaId fila); 
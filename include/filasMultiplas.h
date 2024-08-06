#ifndef FILAS_MULTIPLAS_H
#define FILAS_MULTIPLAS_H

#include "filasID.h"

#include <stdbool.h>

// Definindo uma estrutura para armazenar as filas multiplas

typedef TfilasPrioridades* PfilasPrioridades;
typedef struct {
    int numCPUs;     // Numero de CPUs
    int vetorPrioridades[4]; // Vetor de prioridades
    int vetBloqueados[4]; // Vetor de bloqueados
}TfilasPrioridades;


/*

    Inicializa as filas multiplas
    cada vetor é uma fila
    quando acaba o slot de tempo, se nao terminou 


*/
// tem que ter uma função que vai ser chamado pela cpu pra retorna o processo da vez

// cpu chama funçao que tira processo da lista de filas multiplas

// 

// 







// // Funcao para inicializar as filas multiplas
// FilasMultiplas* criarFilasMultiplas(int numFilas, int capacidade);

// // Funcao para enfileirar um elemento em uma fila especifica
// bool enfileirar(FilasMultiplas* fm, int indiceFila, int valor);

// // Funcao para desenfileirar um elemento de uma fila especifica
// int desenfileirar(FilasMultiplas* fm, int indiceFila, bool *sucesso);

// // Funcao para verificar se uma fila especifica esta vazia
// bool estaVazia(FilasMultiplas* fm, int indiceFila);

// // Funcao para verificar se uma fila especifica esta cheia
// bool estaCheia(FilasMultiplas* fm, int indiceFila);

// // Funcao para liberar a memoria alocada para as filas multiplas
// void liberarFilasMultiplas(FilasMultiplas* fm);

// #endif // FILAS_MULTIPLAS_H



/// Cada classe pode ser um enum

// funcçao pra fazer a conversao da prioridade em unidades de tempo (0-1, 1-2, 2-4, 3-8,)

// usou a fatia inteira = prioridade diminui
// processo entrou para bloqueado = prioridade aumenta

// filas multiplas será vetor
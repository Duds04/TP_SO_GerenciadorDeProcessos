#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "disco.h"

// Wrapper para o malloc que reporta falhas de alocação
static void *safeAlloc(size_t size) {
    void *ptr = malloc(size);
    if(ptr == NULL) {
        fprintf(stderr, "[!] Falha em alocar memória\n");
        exit(65);
    }
    return ptr;
}

void discoInicia(Disco *disco) {
    disco->conteudo = (uint8_t **) safeAlloc(DISCO_INICIAL * sizeof(uint8_t *));
    disco->capacidade = DISCO_INICIAL;
    disco->tamanho = 0;
}

// Transfere uma sequência de páginas para o disco dado seu tamanho em bytes,
// retornando seu índice no disco
int discoTransfere(Disco *disco, const uint8_t pags[], int len) {
    // Procura no vetor por um buraco, uma posição com NULL
    int addr = -1;
    for(int i = 0; i < disco->tamanho; ++i)
        if(disco->conteudo[i] == NULL) {
            addr = i;
            break;
        }
    if(addr < 0) {
        // Nenhum buraco encontrado, adiciona-se um novo ponteiro
        if(disco->tamanho + 1 > disco->capacidade) {
            disco->capacidade *= 2;
            uint8_t **ptr = realloc(disco->conteudo, disco->capacidade * sizeof(uint8_t *));
            if(ptr == NULL) {
                fprintf(stderr, "[!] Falha em alocar memória\n");
                exit(65);
            }
            disco->conteudo = ptr;
        }
        addr = disco->tamanho++;
    }
    disco->conteudo[addr] = safeAlloc(len * sizeof(uint8_t));
    memcpy(disco->conteudo[addr], pags, len * sizeof(uint8_t));
    return addr;
}

// Retira uma sequência de páginas do disco para um vetor
void discoRetira(Disco *disco, int addr, uint8_t vec[], int len) {
    if(addr < 0 || addr >= disco->tamanho) {
        fprintf(stderr, "[!] Endereço de disco inválido %d\n", addr);
        exit(75);
    }
    memcpy(vec, disco->conteudo[addr], len * sizeof(uint8_t));
    free(disco->conteudo[addr]);
    disco->conteudo = NULL;
}

void discoLibera(Disco *disco) {
    for(int i = 0; i < disco->tamanho; ++i)
        free(disco->conteudo[i]);
    free(disco->conteudo);
}

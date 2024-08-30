#ifndef DISCO_H
#define DISCO_H

#include <stdint.h>
#define DISCO_INICIAL 4

// Disco simulado para liberação de espaço na memória principal. Para simular
// a questão de o disco ser muito maior que a memória RAM, ele é representado
// como um vetor dinâmico de ponteiros, onde cada ponteiro apontará para uma
// região de memória contendo as páginas de um processo
typedef struct {
    uint8_t **conteudo;
    int capacidade;
    int tamanho;
} Disco;

void discoInicia(Disco *disco);

// Transfere uma sequência de páginas para o disco dado seu tamanho em bytes,
// retornando seu endereço de disco
int discoTransfere(Disco *disco, const uint8_t pags[], int len);

// Retira uma sequência de páginas do disco para um vetor
void discoRetira(Disco *disco, int addr, uint8_t vec[], int len);

void discoLibera(Disco *disco);

#endif // DISCO_H

#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdint.h>

// Tamanho da memória principal (4KiB) e de cada página (256B)
// Com essa configuração, são 16 páginas no total, o que torna necessário o uso
// de exatamente 16 bits para controle de páginas livres e ocupadas
#define TAMANHO_MEM 4096
#define TAMANHO_PAG  256
typedef uint16_t bitmap_t;

// Memória principal, com metadados de gerência
typedef struct {
    uint8_t conteudo[TAMANHO_MEM];
    bitmap_t ocupadas; // bitmap de controle de páginas livres
    int ultimaPos; // usado no nextfit que guarda ultima posição alocada
} Memoria;

void memoriaInicia(Memoria *mem);

// Aloca um vetor de n variáveis inteiras (4 bytes cada) na memória principal
int32_t *memoriaAloca(Memoria *mem, int n);

// Desaloca um vetor de n variáveis inteiras (4 bytes cada) da memória
// principal, dado um ponteiro para sua localização
void memoriaLibera(Memoria *mem, int n, int32_t *ptr);

int32_t *firstfit(Memoria *mem, int n);


#endif // MEMORIA_H

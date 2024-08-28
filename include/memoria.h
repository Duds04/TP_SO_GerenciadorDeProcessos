#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdint.h>

// Estratégias possíveis de alocação de memória
typedef enum {
    ALOC_FIRST_FIT = 0,
    ALOC_NEXT_FIT,
    ALOC_BEST_FIT,
    ALOC_WORST_FIT,
    NUM_ALOCS,
} AlocID;

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
    int ultimaPos;     // índice da última alocação
    AlocID alocId;     // estratégia de alocação que deve ser utilizada
} Memoria;

void memoriaInicia(Memoria *mem, AlocID alocId);

// Aloca um vetor de n variáveis inteiras (4 bytes cada) na memória principal
int32_t *memoriaAloca(Memoria *mem, int n);

// Desaloca um vetor de n variáveis inteiras (4 bytes cada) da memória
// principal, dado um ponteiro para sua localização
void memoriaLibera(Memoria *mem, int n, int32_t *ptr);

#endif // MEMORIA_H

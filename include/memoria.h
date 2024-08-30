#ifndef MEMORIA_H
#define MEMORIA_H

#include <stdint.h>
#include "paginas.h"

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
#define NUM_PAGINAS (TAMANHO_MEM / TAMANHO_PAG)
typedef uint16_t bitmap_t;

// Memória principal, com metadados de gerência
typedef struct {
    ProcessoPagInfo *dono[NUM_PAGINAS];
    uint8_t conteudo[TAMANHO_MEM];
    bitmap_t ocupadas; // bitmap de controle de páginas livres
    int ultimaPos;     // índice da última alocação
    AlocID alocId;     // estratégia de alocação que deve ser utilizada
} Memoria;

// Calcula o número de páginas a partir do número de variáveis inteiras,
// assumindo que cada variável ocupa 4 bytes
int numPaginasVar(int n);

void memoriaInicia(Memoria *mem, AlocID alocId);

// Retorna um ponteiro para a página de memória informada, trazendo-a do disco
// se for necessário
int32_t *memoriaAcessa(Memoria *mem, ProcessoPagInfo *pags);

// Retorna um ponteiro para a página de memória informada, esteja ela no disco ou não
int32_t *memoriaAcessaConst(const Memoria *mem, ProcessoPagInfo pags);

// Desaloca uma sequência de páginas da memória (principal ou não)
void memoriaLibera(Memoria *mem, ProcessoPagInfo pags);

#endif // MEMORIA_H

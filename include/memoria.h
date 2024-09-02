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
    int somaFragmentosExternos;    // número de todos os fragmentos externos a cada operação
    int somaNosPercorridos;       // número de nós percorridos em todas as operações
    int numOperacoes; // número de operações de alocação e desalocação
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

// Copia conteúdo de uma sequência de páginas para outra. Aloca caso seja
// necessário
void memoriaCopia(Memoria *mem, ProcessoPagInfo *dst, ProcessoPagInfo src);

// Desaloca uma sequência de páginas da memória (principal ou não)
void memoriaLibera(Memoria *mem, ProcessoPagInfo pags);

// Imprime informações sobre a memória
void memoriaImprime(const Memoria *mem);

// conta o número de fragmentos externos na memória
int memoriaFragmentosExternos(const Memoria *mem);

// Imprime informações sobre a paginação
static void imprimePagina(int pg, const Memoria *mem);

// Imprime o bitmap de páginas
static void imprimeBitMap(bitmap_t bitmap);

// Imprime a média de fragmentos externos
void imprimeMediaFragmentosExternos(const Memoria *mem);

// Imprime a média de nós percorridos
void imprimeMediaNosPercorridos(const Memoria *mem);

#endif // MEMORIA_H

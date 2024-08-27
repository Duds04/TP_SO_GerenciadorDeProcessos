#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "memoria.h"

#define INC_MOD(i, cap) (((i) + 1) & ((cap) - 1))

// Teto da divisão inteira
static inline int teto(int a, int b) {
    return a / b + (a % b != 0);
}

// Calcula o número de páginas a partir do número de variáveis inteiras,
// assumindo que cada variável ocupa 4 bytes
static inline int numPaginasVar(int n) {
    return teto(n * 4, TAMANHO_PAG);
}

// Retorna um ponteiro para a i-ésima página de memória
static inline uint8_t *acessaPagina(Memoria *mem, int i) {
    return &mem->conteudo[i * TAMANHO_PAG];
}

// Constrói uma máscara de 16 bits com uma sequência de n 1s a partir do
// i-ésimo bit (inclusive), com 0s em todos os demais bits. Ambos os parâmetros
// devem ser menores ou iguais a dezesseis e positivos! Assume que n >= 1
static uint16_t mascaraSeq(int n, int i) {
    uint16_t mascara = 1;
    for(int b = 0; b < n - 1; ++b)
        mascara = (mascara << 1) & 1;
    mascara <<= i;
    return mascara;
}

void memoriaInicia(Memoria *mem) {
    memset(mem->conteudo, 0, TAMANHO_MEM * sizeof(uint8_t));
    // Todas as páginas começam livres
    mem->ocupadas = 0;
    mem->ultimaPos = 0;
}

// Aloca um vetor de n variáveis inteiras (4 bytes cada) na memória principal
int32_t *memoriaAloca(Memoria *mem, int n) {

    return firstfit(mem, n); // retorna função

}

// Desaloca um vetor de n variáveis inteiras (4 bytes cada) da memória
// principal, dado um ponteiro para sua localização
void memoriaLibera(Memoria *mem, int n, int32_t *fakePtr) {
    uint8_t *ptr = (uint8_t *) fakePtr;
    if(ptr < mem->conteudo || ptr >= mem->conteudo + TAMANHO_MEM) {
        fprintf(stderr, "[!] Função memoriaLibera recebeu um ponteiro inválido\n");
        return;
    }
    int indice = ptr - mem->conteudo;
    int indicePagina = indice / TAMANHO_PAG;
    int numPaginas = numPaginasVar(n);
    mem->ocupadas &= ~mascaraSeq(numPaginas, indicePagina);
}

int32_t *firstfit(Memoria *mem, int n){
    // se precisar alocar 0 espaços, não aloca
    if(n == 0) return NULL;
    int numPaginas = numPaginasVar(n);

    // Encontra essa quantidade de páginas livres na memória. (first-fit)
    int contaLivres = 0, primeiraLivre = -1, ocupadas = mem->ocupadas;
    for(int i = 0; i < 16; ++i) {
        // Percorre o bitmap bit a bit, armazenando o valor de cada um em
        // paginaOcupada. Bit ligado => página correspondente não está livre

        // ele ta literalmente fazendo isso paginaOcupada = ocupadas[i]
        int paginaOcupada = ocupadas & 1;
        ocupadas >>= 1;
        if(!paginaOcupada) {
            ++contaLivres;
            if(primeiraLivre < 0) primeiraLivre = i;
        } else {
            primeiraLivre = -1;
            contaLivres = 0;
        }
        if(contaLivres == numPaginas) {
            // Foi encontrada uma sequência de páginas livres adequada!
            // Começa a partir da posição primeiraLivre. Essas páginas serão
            // marcadas como ocupadas
            mem->ocupadas |= mascaraSeq(numPaginas, primeiraLivre);
            return (int32_t *) acessaPagina(mem, primeiraLivre);
        }
    }
    return NULL; // não foi possível alocar...
}

int32_t *nextfit(Memoria *mem, int n){

    if(n == 0) return NULL;
    int numPaginas = numPaginasVar(n);
    
    int contaLivres = 0, primeiraLivre = -1, ocupadas = mem->ocupadas;
    for(int i = 0; i < 16; ++i) {
        // Percorre o bitmap bit a bit, armazenando o valor de cada um em
        // paginaOcupada. Bit ligado => página correspondente não está livre

        // ele ta literalmente fazendo isso paginaOcupada = ocupadas[i]
        int paginaOcupada = ocupadas & 1;
        ocupadas >>= 1;
        if(!paginaOcupada) {
            ++contaLivres;
            if(primeiraLivre < 0) primeiraLivre = i;
        } else {
            primeiraLivre = -1;
            contaLivres = 0;
        }
        if(contaLivres == numPaginas) {
            // Foi encontrada uma sequência de páginas livres adequada!
            // Começa a partir da posição primeiraLivre. Essas páginas serão
            // marcadas como ocupadas
            mem->ocupadas |= mascaraSeq(numPaginas, primeiraLivre);
            return (int32_t *) acessaPagina(mem, primeiraLivre);
        }
    }
    return NULL; // não foi possível alocar...
}
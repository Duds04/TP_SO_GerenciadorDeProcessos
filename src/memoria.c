#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "memoria.h"

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

void memoriaInicia(Memoria *mem, AlocID alocId) {
    memset(mem->conteudo, 0, TAMANHO_MEM * sizeof(uint8_t));
    mem->ocupadas = 0; // todas as páginas começam livres
    mem->ultimaPos = 0;
    mem->alocId = alocId;
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

// First-fit: encaixa a alocação requisitada no primeiro espaço adequado
static int32_t *firstfit(Memoria *mem, int n) {
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

// Next-fit: encaixa a alocação requisitada no primeiro espaço adequado a
// partir da última alocação
static int32_t *nextfit(Memoria *mem, int n) {

    if(n == 0) return NULL;
    int numPaginas = numPaginasVar(n);

    int contaLivres = 0, primeiraLivre = -1, ocupadas = mem->ocupadas;

    // Variável usada para salvar o índice da primeira ocorrencia de 1
    int primeiro1 = -1;

    // verifica se há espaço para alocação da última posição livre até
    // o fim (next fit)
    ocupadas <<= mem->ultimaPos; // joga um monte de bits fora?
    for(int i = mem->ultimaPos; i > mem->ultimaPos-1; i = (i+1)%16) {
        // Percorre o bitmap bit a bit, armazenando o valor de cada um em
        // paginaOcupada. Bit ligado => página correspondente não está livre

        // ele ta literalmente fazendo isso paginaOcupada = ocupadas[i]
        int paginaOcupada = ocupadas & 1;
        ocupadas >>= 1;
        if(!paginaOcupada) {
            ++contaLivres;
            if(primeiraLivre < 0) primeiraLivre = i;
        } else {
            if(primeiro1 > 0) primeiro1 = i;
            primeiraLivre = -1;
            contaLivres = 0;
        }
        if(contaLivres == numPaginas) {
            // Foi encontrada uma sequência de páginas livres adequada!
            // Começa a partir da posição primeiraLivre. Essas páginas serão
            // marcadas como ocupadas
            mem->ocupadas |= mascaraSeq(numPaginas, primeiraLivre);
            mem->ultimaPos = primeiraLivre + numPaginas;
            return (int32_t *) acessaPagina(mem, primeiraLivre);
        }

    }

    // se não há espaço e houve ocorrencia de 1, verifica espaços livres até essa
    // primeira ocorrencia

    // se não há espaço e não houve ocorrencia de 1s significa que o espaço restante
    // é vazio e não é suficiente
    if(primeiro1 > 0) primeiro1 = 16;

    // TODO: socorro dudu
    // é necessário verificar tudo?
    // da forma que o algorítmo funciona sim, mas podemos pensar em uma alteração
    // em que atualiza o mem->ultimaPos quando se desalocar um valor, mas fugiria
    // do algorítmo

    contaLivres = 0;
    primeiraLivre = -1;
    ocupadas = mem->ocupadas;
    for(int i = 0; i < primeiro1; i++) {
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
            mem->ultimaPos = primeiraLivre + numPaginas;
            return (int32_t *) acessaPagina(mem, primeiraLivre);
        }

    }

    return NULL; // não foi possível alocar...
}

// Best-fit: encaixa a alocação requisitada no menor espaço adequado
static int32_t *bestfit(Memoria *mem, int n) {

    if(n == 0) return NULL;
    int numPaginas = numPaginasVar(n);

    int contaLivres = 0, melhorLugar = -1, ocupadas = mem->ocupadas, melhorEspaco = 17, lugarAtual=-1;

    for(int i = 0; i < 16; ++i) {
        // Percorre o bitmap bit a bit, armazenando o valor de cada um em
        // paginaOcupada. Bit ligado => página correspondente não está livre

        // ele ta literalmente fazendo isso paginaOcupada = ocupadas[i]
        int paginaOcupada = ocupadas & 1;
        ocupadas >>= 1;
        if(!paginaOcupada) {
            ++contaLivres;
            if(lugarAtual < 0) lugarAtual = i;
        } else {
            // se encontra pagina ocupada verifica se é um espaço melhor
            if (contaLivres>=numPaginas && contaLivres < melhorEspaco) {
                melhorEspaco = contaLivres;
                melhorLugar = lugarAtual;
            }
            lugarAtual = -1;
            contaLivres = 0;
        }

    }
    // se chega no fim, verifica se é um espaço melhor
    if(contaLivres >= numPaginas && contaLivres < melhorEspaco) {
        melhorEspaco = contaLivres;
        melhorLugar = lugarAtual;
    }

    // não existe espaço maior que 16
    if(melhorEspaco <= 16) {
        // Foi encontrada uma sequência de páginas livres adequada!
        // Começa a partir da posição melhorLugar. Essas páginas serão
        // marcadas como ocupadas
        mem->ocupadas |= mascaraSeq(numPaginas, melhorLugar);
        return (int32_t *) acessaPagina(mem, melhorLugar);
    }
    return NULL; // não foi possível alocar...
}

// Worst-fit: encaixa a alocação requisitada no maior espaço adequado
static int32_t *worstfit(Memoria *mem, int n) {

    if(n == 0) return NULL;
    int numPaginas = numPaginasVar(n);

    int contaLivres = 0, piorLugar = -1, ocupadas = mem->ocupadas, piorEspaco = numPaginas-1, lugarAtual=-1;

    for(int i = 0; i < 16; ++i) {
        // Percorre o bitmap bit a bit, armazenando o valor de cada um em
        // paginaOcupada. Bit ligado => página correspondente não está livre

        // ele ta literalmente fazendo isso paginaOcupada = ocupadas[i]
        int paginaOcupada = ocupadas & 1;
        ocupadas >>= 1;
        if(!paginaOcupada) {
            ++contaLivres;
            if(lugarAtual < 0) lugarAtual = i;
        } else {
            // se encontra pagina ocupada verifica se é um espaço pior
            if (contaLivres >= numPaginas && contaLivres > piorEspaco) {
                piorEspaco = contaLivres;
                piorLugar = lugarAtual;
            }
            lugarAtual = -1;
            contaLivres = 0;
        }

    }
    // se chega no fim, verifica se é um espaço pior
    if(contaLivres>=numPaginas && contaLivres > piorEspaco) {
        piorEspaco = contaLivres;
        piorLugar = lugarAtual;
    }

    if(piorEspaco > numPaginas-1) {
        // Foi encontrada uma sequência de páginas livres adequada!
        // Começa a partir da posição piorLugar. Essas páginas serão
        // marcadas como ocupadas
        mem->ocupadas |= mascaraSeq(numPaginas, piorLugar);
        return (int32_t *) acessaPagina(mem, piorLugar);
    }
    return NULL; // não foi possível alocar...
}

// Aloca um vetor de n variáveis inteiras (4 bytes cada) na memória principal
int32_t *memoriaAloca(Memoria *mem, int n) {
    switch(mem->alocId) {
        case ALOC_FIRST_FIT: return firstfit(mem, n);
        case ALOC_NEXT_FIT:  return nextfit(mem, n);
        case ALOC_BEST_FIT:  return bestfit(mem, n);
        case ALOC_WORST_FIT: return worstfit(mem, n);
        default:
            fprintf(stderr, "[!] Estratégia de alocação não reconhecida\n");
            exit(65);
    }
}


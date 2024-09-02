#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "memoria.h"
#include "paginas.h"

// Teto da divisão inteira
static inline int teto(int a, int b) {
    return a / b + (a % b != 0);
}

// Calcula o número de páginas a partir do número de variáveis inteiras,
// assumindo que cada variável ocupa 4 bytes
int numPaginasVar(int n) {
    return teto(n * 4, TAMANHO_PAG);
}

// Constrói uma máscara de 16 bits com uma sequência de n 1s a partir do
// i-ésimo bit (inclusive), com 0s em todos os demais bits. Ambos os parâmetros
// devem ser menores ou iguais a dezesseis e positivos! Assume que n >= 1
static uint16_t mascaraSeq(int n, int i) {
    uint16_t mascara = 1;
    for(int b = 0; b < n - 1; ++b)
        mascara = (mascara << 1) | 1;
    mascara <<= i;
    return mascara;
}

void memoriaInicia(Memoria *mem, AlocID alocId) {
    for(int i = 0; i < NUM_PAGINAS; ++i)
        mem->dono[i] = NULL;
    memset(mem->conteudo, 0, TAMANHO_MEM * sizeof(uint8_t));
    mem->ocupadas = 0; // todas as páginas começam livres
    mem->ultimaPos = 0;
    mem->alocId = alocId;
}

// Retorna um ponteiro para a página de memória informada, esteja ela no disco ou não
int32_t *memoriaAcessaConst(const Memoria *mem, ProcessoPagInfo pags) {
    if(pags.paginaInicial < 0 || pags.numPaginas < 0
            || pags.numPaginas >= NUM_PAGINAS) return NULL;
    return (int32_t *) &mem->conteudo[pags.paginaInicial * TAMANHO_PAG];
}

// Desaloca uma sequência de páginas da memória (principal ou não)
void memoriaLibera(Memoria *mem, ProcessoPagInfo pags) {
    if(pags.paginaInicial < 0) return; // nada a liberar
    if(pags.numPaginas >= NUM_PAGINAS) {
        fprintf(stderr, "[!] Informação de página inválida dada para memoriaLibera\n");
        return;
    }
    mem->ocupadas &= ~mascaraSeq(pags.numPaginas, pags.paginaInicial);
    mem->dono[pags.paginaInicial] = NULL;
}

// First-fit: encaixa a alocação requisitada no primeiro espaço adequado
static ProcessoPagInfo firstfit(Memoria *mem, int numPaginas) {
    // se precisar alocar 0 espaços, não aloca
    if(numPaginas == 0) return PAGINA_NULA;

    // Encontra essa quantidade de páginas livres na memória. (first-fit)
    int contaLivres = 0, primeiraLivre = -1, ocupadas = mem->ocupadas;
    for(int i = 0; i < NUM_PAGINAS; ++i) {
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
            ProcessoPagInfo pags = {
                .paginaInicial = primeiraLivre,
                .numPaginas = numPaginas,
                .noDisco = false,
            };
            return pags;
        }
    }
    return PAGINA_NULA; // não foi possível alocar...
}

// Next-fit: encaixa a alocação requisitada no primeiro espaço adequado a
// partir da última alocação
static ProcessoPagInfo nextfit(Memoria *mem, int numPaginas) {
    if(numPaginas == 0) return PAGINA_NULA;

    int contaLivres = 0, primeiraLivre = -1, ocupadas = mem->ocupadas;

    // Variável usada para salvar o índice da primeira ocorrencia de 1
    int primeiro1 = -1;

    // verifica se há espaço para alocação da última posição livre até
    // o fim (next fit)
    ocupadas >>= mem->ultimaPos;
    for(int i = mem->ultimaPos; i < NUM_PAGINAS; ++i) {
        // Percorre o bitmap bit a bit, armazenando o valor de cada um em
        // paginaOcupada. Bit ligado => página correspondente não está livre

        // ele ta literalmente fazendo isso paginaOcupada = ocupadas[i]
        int paginaOcupada = ocupadas & 1;
        ocupadas >>= 1;
        if(!paginaOcupada) {
            ++contaLivres;
            if(primeiraLivre < 0) primeiraLivre = i;
        } else {
            if(primeiro1 < 0) primeiro1 = i;
            primeiraLivre = -1;
            contaLivres = 0;
        }
        if(contaLivres == numPaginas) {
            // Foi encontrada uma sequência de páginas livres adequada!
            // Começa a partir da posição primeiraLivre. Essas páginas serão
            // marcadas como ocupadas
            mem->ocupadas |= mascaraSeq(numPaginas, primeiraLivre);
            mem->ultimaPos = primeiraLivre + numPaginas;
            ProcessoPagInfo pags = {
                .paginaInicial = primeiraLivre,
                .numPaginas = numPaginas,
                .noDisco = false,
            };
            return pags;
        }

    }
    // se não há espaço e houve ocorrencia de 1, verifica espaços livres até essa
    // primeira ocorrencia

    // se não há espaço e não houve ocorrencia de 1s significa que o espaço restante
    // é vazio e não é suficiente
    if(primeiro1 > 0) primeiro1 = NUM_PAGINAS;

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
            ProcessoPagInfo pags = {
                .paginaInicial = primeiraLivre,
                .numPaginas = numPaginas,
                .noDisco = false,
            };
            return pags;
        }
    }
    return PAGINA_NULA; // não foi possível alocar...
}

// Best-fit: encaixa a alocação requisitada no menor espaço adequado
static ProcessoPagInfo bestfit(Memoria *mem, int numPaginas) {
    if(numPaginas == 0) return PAGINA_NULA;

    int melhorLugar = -1, melhorEspaco = 17;
    int contaLivres = 0, ocupadas = mem->ocupadas, lugarAtual=-1;

    for(int i = 0; i < NUM_PAGINAS; ++i) {
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
    if(melhorEspaco <= NUM_PAGINAS) {
        // Foi encontrada uma sequência de páginas livres adequada!
        // Começa a partir da posição melhorLugar. Essas páginas serão
        // marcadas como ocupadas
        mem->ocupadas |= mascaraSeq(numPaginas, melhorLugar);
        ProcessoPagInfo pags = {
            .paginaInicial = melhorLugar,
            .numPaginas = numPaginas,
            .noDisco = false,
        };
        return pags;
    }
    return PAGINA_NULA; // não foi possível alocar...
}

// Worst-fit: encaixa a alocação requisitada no maior espaço adequado
static ProcessoPagInfo worstfit(Memoria *mem, int numPaginas) {
    if(numPaginas == 0) return PAGINA_NULA;

    int piorLugar = -1, piorEspaco = numPaginas - 1;
    int contaLivres = 0, ocupadas = mem->ocupadas, lugarAtual=-1;

    for(int i = 0; i < NUM_PAGINAS; ++i) {
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
        ProcessoPagInfo pags = {
            .paginaInicial = piorLugar,
            .numPaginas = numPaginas,
            .noDisco = false,
        };
        return pags;
    }
    return PAGINA_NULA; // não foi possível alocar...
}

// Aloca uma sequência de páginas na memória principal que comporte n variáveis
// inteiras. Assume que há espaço na memória principal para a alocação
static ProcessoPagInfo memoriaAloca(Memoria *mem, int numPaginas) {
    if(numPaginas < 0) return PAGINA_NULA;
    ProcessoPagInfo pags;
    switch(mem->alocId) {
        case ALOC_FIRST_FIT: pags = firstfit(mem, numPaginas); break;
        case ALOC_NEXT_FIT:  pags = nextfit(mem, numPaginas);  break;
        case ALOC_BEST_FIT:  pags = bestfit(mem, numPaginas);  break;
        case ALOC_WORST_FIT: pags = worstfit(mem, numPaginas); break;
        default:
            fprintf(stderr, "[!] Estratégia de alocação não reconhecida\n");
            exit(65);
    }
    if(pags.paginaInicial < 0) {
        fprintf(stderr, "[!] Sem memória principal suficiente\n");
        exit(75);
    }
    return pags;
}

// Retorna um ponteiro para a página de memória informada
int32_t *memoriaAcessa(Memoria *mem, ProcessoPagInfo *pags) {
    if(pags->paginaInicial < 0) {
        // Paginação por demanda: devemos alocar uma nova sequência de páginas
        // quando ocorre uma falha de página como essa
        *pags = memoriaAloca(mem, pags->numPaginas);
        mem->dono[pags->paginaInicial] = pags;
    }
    return (int32_t *) &mem->conteudo[pags->paginaInicial * TAMANHO_PAG];
}

// Copia conteúdo de uma sequência de páginas para outra. Aloca caso seja
// necessário
void memoriaCopia(Memoria *mem, ProcessoPagInfo *dst, ProcessoPagInfo src) {
    if(src.paginaInicial < 0) return; // nada a copiar
    dst->numPaginas = src.numPaginas;
    memcpy(memoriaAcessa(mem, dst), memoriaAcessa(mem, &src),
            src.numPaginas * TAMANHO_PAG);
}

static void imprimePagina(int pg, const Memoria *mem) {
    const int linhas = 8;
    int32_t *pag = (int32_t *) &mem->conteudo[pg * TAMANHO_PAG];
    int varPorLinha = TAMANHO_PAG / (4 * linhas);
    for(int i = 0; i < linhas; ++i) {
        printf("%4d", pag[i * linhas]);
        for(int j = 1; j < varPorLinha; ++j)
            printf(" %4d ", pag[i * linhas + j]);
        printf("\n");
    }
}

static void imprimeBitMap(bitmap_t bitmap){
    int i, r;
    for(i = NUM_PAGINAS; i >= 0; i--) {
        // Executa a operação shift right até a última posição da direita
        // para cada bit.
        r = bitmap >> i;
        if(r & 1) printf("1");
        else printf("0");
    }
    printf("\n");
}

void memoriaImprime(const Memoria *mem){
    printf("\tBitmap da memória:\n");
    imprimeBitMap(mem->ocupadas);

    printf("\tInformações de pagina dos processos\n");
    for(int i = 0; i < NUM_PAGINAS; i++) {
        int pgInicial = mem->dono[i]->paginaInicial;
        int numPg = mem->dono[i]->numPaginas;

        printf("Pagina inicial: %d\nNumero de paginas: %d\n",
                pgInicial, numPg);

        if(mem->dono[i]->noDisco) {
            printf("Em disco\n");
        } else {
            printf("Em memória\n");
            for(int j = pgInicial; j < pgInicial + numPg; j++){
                printf("Pagina %d\n", i);
                imprimePagina(i, mem);
            }
        }

    }
}

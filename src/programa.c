#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "instrucao.h"
#include "programa.h"

void programaInicia(Programa *prog) {
    Instrucao *listInstruct;
    listInstruct = (Instrucao *) malloc(TAM_INIT * sizeof(Instrucao));
    if (listInstruct == NULL) {
        printf("Erro ao alocar memória!");
        exit(1);
    }
    prog->tamanho = TAM_INIT;
    prog->intrucoes = listInstruct;
    prog->ultimo = 0;
    prog->numRegs = 0;
}

static void realoca(Programa *prog) {
    prog->intrucoes = realloc(prog->intrucoes, prog->tamanho*2 * sizeof(Instrucao));

    if (prog->intrucoes == NULL) {
        printf("Erro ao realocar memória!");
        exit(1);
    }
    prog->tamanho = prog->tamanho*2;
}

void programaAdiciona(Programa *prog, Instrucao instrucao) {
    if(prog->ultimo >= prog->tamanho) realoca(prog);
    prog->intrucoes[prog->ultimo] = instrucao;
    prog->ultimo++;
}

void programaImprime(const Programa *prog, int inicio) {
    for(int i=inicio; i<prog->ultimo; i++) {
        instrucaoImprime(&prog->intrucoes[i]);
    }
}

// Faz uma cópia profunda da lista de instruções
void programaCopia(const Programa *src, Programa *dst) {
    Instrucao *aux = (Instrucao*) malloc(src->tamanho * sizeof(Instrucao));
    if(aux == NULL) {
        printf("Erro ao alocar memória!");
        exit(1);
    }
    memcpy(aux, src->intrucoes, src->ultimo * sizeof(Instrucao));
    dst->intrucoes = aux;
    dst->ultimo = src->ultimo;
    dst->tamanho = src->tamanho;
    dst->numRegs = src->numRegs;
}

// Carrega um arquivo completo como uma lista de instruções
void programaCarrega(Programa *prog, FILE *arq) {
    int reg = 0;
    char linha[64];
    Instrucao inst;
    while(fgets(linha, 64, arq) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        // Carrega instruções ignorando as inválidas
        if(instrucaoCarrega(&inst, linha) != 0) continue;
        if(inst.op == 'N') {
            // A instrução N é especial, pois dá informações que são necessárias
            // para a criação do processo. Por isso, ela é tratada aqui e não
            // tem efeito nenhum em tempo de execução
            reg += inst.arg0; // aumenta número de registradores
        }
        programaAdiciona(prog, inst);
    }
    prog->numRegs = reg;
}

void programaLibera(Programa *prog) {
    free(prog->intrucoes);
}

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "instrucao.h"

// Carrega uma instrução a partir da linha correspondente no código, retornando
// um valor diferente de zero em caso de instrução inválida
int instrucaoCarrega(Instrucao *inst, const char *linha) {
    // Cada linha no código deve ser iniciada pelo código de operação, seguido
    // dos seus argumentos separados por espaços em branco. O número e a
    // natureza desses argumentos dependem da operação
    int i = 1;
    inst->op = linha[0];
    inst->arq[0] = '\0';
    while(isspace(linha[i])) ++i; // pula espaços após o código
    switch(inst->op) {
        case 'T':
            // Sem argumentos de qualquer natureza
            break;
        case 'N':
        case 'D':
        case 'B':
        case 'F':
            // Um argumento numérico
            sscanf(&linha[i], "%d", &inst->arg0);
            break;
        case 'V':
        case 'A':
        case 'S':
            // Dois argumentos numéricos
            // NOTA: a chamada ao sscanf abaixo usa espaços em branco (qualquer
            // número deles) para diferenciar um número do próximo, que é
            // exatamente o que a especificação quer
            sscanf(&linha[i], "%d%d", &inst->arg0, &inst->arg1);
            break;
        case 'R':
            // Um argumento textual
            // NOTA: não se deve usar strcpy jamais; veja man strcpy para os
            // problemas se segurança dessa função. strncpy é a versão decente
            strncpy(inst->arq, &linha[i], TAM_INIT);
            break;
        default:
            // Código de operação não reconhecido...
            return 1;
    }
    return 0;
}

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

void instrucaoImprime(const Instrucao *inst) {
    switch (inst->op) {
        case 'T':
            printf("%c\n", inst->op);
            break;
        case 'N':
        case 'D':
        case 'B':
        case 'F':
            printf("%c %d\n", inst->op, inst->arg0);
            break;
        case 'V':
        case 'A':
        case 'S':
            printf("%c %d %d\n", inst->op, inst->arg0, inst->arg1);
            break;
        case 'R':
            printf("%c %s\n", inst->op, inst->arq);
            break;
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
}

// Carrega um arquivo completo como uma lista de instruções. Retorna um número
// positivo de registradores que devem ser alocados para o programa
int programaCarrega(Programa *prog, FILE *arq) {
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
    return reg;
}

void programaLibera(Programa *prog) {
    free(prog->intrucoes);
}

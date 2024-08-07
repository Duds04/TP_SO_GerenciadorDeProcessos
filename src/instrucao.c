#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "instrucao.h"

// Carrega uma instrução a partir da linha correspondente no código, retornando
// um valor diferente de zero em caso de instrução inválida
int carrega_instrucao(Instrucao *inst, const char *linha) {
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

void liIniciaLista(TListaInstrucao *pLista){
    Instrucao *listInstruct;

    listInstruct = (Instrucao *) malloc(TAM_INIT * sizeof(Instrucao));

    if (listInstruct == NULL) {
        printf("Erro ao alocar memória!");
        exit(1);
    }

    pLista->tamanho = TAM_INIT;
    pLista->intrucoes = listInstruct;
    pLista->ultimo = 0;
}

static void realoca(TListaInstrucao *pLista) {
    pLista->intrucoes = realloc(pLista->intrucoes, pLista->tamanho*2 * sizeof(Instrucao));

    if (pLista->intrucoes == NULL) {
        printf("Erro ao realocar memória!");
        exit(1);
    }
    pLista->tamanho = pLista->tamanho*2;
}

void liInsereFinal(TListaInstrucao *pLista, Instrucao instrucao){
    if(pLista->ultimo >= pLista->tamanho) realoca(pLista);
    pLista->intrucoes[pLista->ultimo] = instrucao;
    pLista->ultimo++;
}

void liImprimeLista(const TListaInstrucao *pLista, int inicio){
    for(int i=inicio; i<pLista->ultimo; i++){
        liImprimeInstrucao(pLista->intrucoes[i]);
    }
}

void liImprimeInstrucao(Instrucao instrucao){
    switch (instrucao.op){
        case 'T':
            printf("%c\n", instrucao.op);
            break;
        case 'N':
        case 'D':
        case 'B':
        case 'F':
            printf("%c %d\n", instrucao.op, instrucao.arg0);
            break;
        case 'V':
        case 'A':
        case 'S':
            printf("%c %d %d\n", instrucao.op, instrucao.arg0, instrucao.arg1);
            break;
        case 'R':
            printf("%c %s\n", instrucao.op, instrucao.arq);
            break;
    }

}

void liLiberaLista(TListaInstrucao *pLista){
    free(pLista->intrucoes);
}

// Faz uma cópia profunda da lista de instruções
void liCopiaProfunda(const TListaInstrucao *src, TListaInstrucao *dst) {
    Instrucao *aux = (Instrucao*) malloc(dst->tamanho * sizeof(Instrucao));
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
int carrega_executavel(TListaInstrucao *prog, FILE *arq) {
    int reg = 0;
    char linha[64];
    Instrucao inst;
    while(fgets(linha, 64, arq) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';
        // Carrega instruções ignorando as inválidas
        if(carrega_instrucao(&inst, linha) != 0) continue;
        if(inst.op == 'N') {
            // A instrução N é especial, pois dá informações que são necessárias
            // para a criação do processo. Por isso, ela é tratada aqui e não
            // tem efeito nenhum em tempo de execução
            reg += inst.arg0; // aumenta número de registradores
        }
        liInsereFinal(prog, inst);
    }
    return reg;
}

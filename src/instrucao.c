#include <stdio.h>
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

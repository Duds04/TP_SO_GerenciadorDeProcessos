#ifndef INSTRUCAO_H
#define INSTRUCAO_H

#define TAM_INIT 32

typedef struct {
    char op;            // código de operação: N, D, V, A, S, B, T, F ou R
    int arg0, arg1;     // argumentos da instrução, na ordem
    char arq[TAM_INIT]; // nome do arquivo a ser carregado; apenas na operação R
} Instrucao;

// Carrega uma instrução a partir da linha correspondente no código, retornando
// um valor diferente de zero em caso de instrução inválida
int instrucaoCarrega(Instrucao *inst, const char *linha);

void instrucaoImprime(const Instrucao *inst);

#endif // INSTRUCOES_H

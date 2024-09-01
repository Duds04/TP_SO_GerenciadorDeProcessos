#ifndef DESEMPENHO_H
#define DESEMPENHO_H

// #include <stdint.h>


typedef struct {
    int fragmentosExt;
    int nosPercoridos;
} Desempenho;

void teste();

// Inicializa a estrutura de desempenho
void desempenhoInicial(Desempenho *desempenho);

// Número Médio de Fragmentos Externos
void calcularFragmentosExt(Desempenho *desempenho, int numProcessos);

// Número Médio de Nós Percorridos
void calcularNosPercorridos(Desempenho *desempenho, int numProcessos);

// Número de Vezes que um Processo foi Levado para o Disco
// void calcularUtilizacaoDisco(Desempenho *desempenho, int numProcessos);

// Imprime a lista de desempenho observada
void desempenhoImprimir(const Desempenho *desempenho);

#endif // DESEMPENHO_H

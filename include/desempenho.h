#ifndef DESEMPENHO_H
#define DESEMPENHO_H

// #include <stdint.h>


typedef struct {
    int fragmentosExt;
    int tempo;
    int nosPercoridos;
    int utilizacaoDisco;
} Desempenho;

void teste();

// Inicializa a estrutura de desempenho
void iniciarDesempenho(Desempenho *desempenho);

// Incrementa o número de fragmentos externos
void incrementarFragmentosExt(Desempenho *desempenho);

// Incrementa o tempo
void incrementarTempo(Desempenho *desempenho);

// Incrementa o número de nós percorridos
void incrementarNosPercoridos(Desempenho *desempenho);

// Incrementa o número de vezes que um processo foi levado para o disco
void incrementarUtilizacaoDisco(Desempenho *desempenho);

// Número Médio de Fragmentos Externos
void calcularFragmentosExt(Desempenho *desempenho, int numProcessos);

// Tempo Médio de Alocação
void calcularTempo(Desempenho *desempenho, int numProcessos);

// Número Médio de Nós Percorridos
void calcularNosPercorridos(Desempenho *desempenho, int numProcessos);

// Número de Vezes que um Processo foi Levado para o Disco
void calcularUtilizacaoDisco(Desempenho *desempenho, int numProcessos);

// Imprime a lista de desempenho observada
void imprimirDesempenho(const Desempenho *desempenho);

#endif // DESEMPENHO_H

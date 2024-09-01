#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "desempenho.h"

void teste(){
    printf("\n\n\nteste\n\n\n");
}

void iniciarDesempenho(Desempenho *desempenho) {
    desempenho->fragmentosExt = 0;
    desempenho->tempo = 0;
    desempenho->nosPercoridos = 0;
    desempenho->utilizacaoDisco = 0;
}

void incrementarFragmentosExt(Desempenho *desempenho) {
    desempenho->fragmentosExt++;
}

void incrementarTempo(Desempenho *desempenho) {
    desempenho->tempo++;
}

void incrementarNosPercoridos(Desempenho *desempenho) {
    desempenho->nosPercoridos++;
}

void incrementarUtilizacaoDisco(Desempenho *desempenho) {
    desempenho->utilizacaoDisco++;
}

void calcularFragmentosExt(Desempenho *desempenho, int numProcessos) {
    desempenho->fragmentosExt /= numProcessos;
}

void calcularTempo(Desempenho *desempenho, int numProcessos) {
    desempenho->tempo /= numProcessos;
}

void calcularNosPercorridos(Desempenho *desempenho, int numProcessos) {
    desempenho->nosPercoridos /= numProcessos;
}

void calcularUtilizacaoDisco(Desempenho *desempenho, int numProcessos) {
    desempenho->utilizacaoDisco /= numProcessos;
}

void imprimirDesempenho(const Desempenho *desempenho) {
    printf("Número Médio de Fragmentos Externos: %d\n", desempenho->fragmentosExt);
    printf("Tempo Médio de Alocação: %d\n", desempenho->tempo);
    printf("Número Médio de Nós Percorridos: %d\n", desempenho->nosPercoridos);
    printf("Número de Vezes que um Processo foi Levado para o Disco: %d\n", desempenho->utilizacaoDisco);
}

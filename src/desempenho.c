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
    desempenho->nosPercoridos = 0;
}

void incrementarFragmentosExt(Desempenho *desempenho) {
int contarFragmentosExternos(int ocupadas) {
    int fragmentos = 0;
    int contaLivres = 0;
    
    for (int i = 0; i < 16; ++i) {
        int paginaOcupada = ocupadas & 1;
        ocupadas >>= 1;
        
        if (!paginaOcupada) {
            if (contaLivres == 0) {
                ++fragmentos;
            }
            ++contaLivres;
        } else {
            contaLivres = 0;
        }
    }
    return fragmentos;
}

void calcularFragmentosExt(Desempenho *desempenho, int numProcessos) {
    desempenho->fragmentosExt /= numProcessos;
}


void calcularNosPercorridos(Desempenho *desempenho, int numProcessos) {
    desempenho->nosPercoridos /= numProcessos;
}

void imprimirDesempenho(const Desempenho *desempenho) {
    printf("Número Médio de Fragmentos Externos: %d\n", desempenho->fragmentosExt);
    printf("Número Médio de Nós Percorridos: %d\n", desempenho->nosPercoridos);
}

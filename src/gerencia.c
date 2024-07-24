#include <stdio.h>
#include <unistd.h>

#include "gerencia.h"
#include "instrucao.h"

#define BUF_MAX 64

// Laço principal da gerência. Recebe ponta de leitura do pipe
void gerencia_main(int controle_fd) {
    FILE *init = fopen("init", "r");
    if(init == NULL) {
        fprintf(stderr, "[!] Arquivo init não pode ser aberto\n");
        return;
    }
    TListaInstrucao programa;
    liIniciaLista(&programa);
    int r = carrega_executavel(&programa, init);
    printf("Instruções de init:\n");
    liImprimeLista(&programa);
    printf("Registradores para init: %d\n", r);
    fclose(init);

    char buf[BUF_MAX];
    while(1) {
        int n = read(controle_fd, buf, BUF_MAX);
        for(int i = 0; i < n; ++i) {
            switch(buf[i]) {
                case 'M':
                    printf("Recebi um M, encerrando\n");
                    return;
                default:
                    printf("Recebi um %c\n", buf[i]);
            }
        }
    }
}

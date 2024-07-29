#include <stdio.h>
#include <unistd.h>

#include "gerencia.h"
#include "instrucao.h"
#include "tabela.h"

#define BUF_MAX 64

// Laço principal da gerência. Recebe ponta de leitura do pipe
void gerencia_main(int controle_fd) {
    FILE *init = fopen("init", "r");
    if(init == NULL) {
        fprintf(stderr, "[!] Arquivo init não pode ser aberto\n");
        return;
    }
    TTabelaProcesso tabela;
    tpIniciaLista(&tabela);

    // Carrega o processo inicial do arquivo init
    TListaInstrucao programa_init;
    liIniciaLista(&programa_init);
    int r = carrega_executavel(&programa_init, init);
    fclose(init);
    tpAdicionaProcesso(&tabela, -1, 0, 10, r, programa_init, 0);
    tpAdicionaProcesso(&tabela, 0, 5, 10, r, programa_init, 5);
    tpImprimeLista(&tabela);

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
                    // Aqui a instrução que o processo recebeu via pipe (recebeu
                    // do processo de controle) deve ser executada
            }
        }
    }
    tpLiberaLista(&tabela);
}

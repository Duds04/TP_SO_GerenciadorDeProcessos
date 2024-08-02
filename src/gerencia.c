#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "gerencia.h"
#include "instrucao.h"
#include "tabela.h"
#include "robin.h"

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

    // Teste da fila robin
    FilaRobin rob;
    robin_inicia(&rob);
    robin_adiciona(&rob, 1);
    robin_adiciona(&rob, 2);
    robin_adiciona(&rob, 3);
    robin_adiciona(&rob, 4);
    robin_adiciona(&rob, 5);
    robin_imprime(&rob);
    robin_adiciona(&rob, 6);
    robin_adiciona(&rob, 7);
    robin_adiciona(&rob, 8);
    robin_adiciona(&rob, 9);
    robin_imprime(&rob);
    assert(robin_retira(&rob) == 1);
    assert(robin_retira(&rob) == 2);
    assert(robin_retira(&rob) == 3);
    assert(robin_retira(&rob) == 4);
    assert(robin_retira(&rob) == 5);
    assert(robin_retira(&rob) == 6);
    assert(robin_retira(&rob) == 7);
    assert(robin_retira(&rob) == 8);
    assert(robin_retira(&rob) == 9);
    robin_imprime(&rob);
    robin_libera(&rob);

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

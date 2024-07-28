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
    // Carrega instruções para a lista e retorna numero de registradores usados
    // nesse programa
    int r = carrega_executavel(&programa, init);
    printf("Instruções de init:\n");
    liImprimeLista(&programa);
    printf("Registradores para init: %d\n", r);
    fclose(init);

    // Teste da lista de bloqueados (só pra provar que eu fiz testes kkkkk),
    // depois dá pra apagar esse código sem culpa
    /*
#include <assert.h>
    ListaBloqueados bloq;
    bloqueados_inicia(&bloq);
    bloqueados_insere(&bloq, 0, 2);
    bloqueados_insere(&bloq, 1, 1);
    bloqueados_insere(&bloq, 2, 2);
    bloqueados_tique(&bloq);
    int p = bloqueados_remove0(&bloq);
    assert(p == 1);
    p = bloqueados_remove0(&bloq);
    assert(p < 0);
    bloqueados_tique(&bloq);
    bloqueados_imprime(&bloq);
    int p0 = bloqueados_remove0(&bloq);
    int p1 = bloqueados_remove0(&bloq);
    int p2 = bloqueados_remove0(&bloq);
    assert(p0 >= 0 && p1 >= 0 && p2 < 0);
    bloqueados_libera(&bloq);
    */

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
}

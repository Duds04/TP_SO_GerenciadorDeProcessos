#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "bloqueados.h"
#include "gerencia.h"
#include "instrucao.h"
#include "tabela.h"
#include "cpu.h"
#include "filasMultiplas.h"

#define BUF_MAX 64

void executaUnidadeTempo(CPU * cpu, PfilasPrioridades filas, ListaBloqueados *listaBloq);

// Laço principal da gerência. Recebe ponta de leitura do pipe e configuração
void gerencia_main(int controle_fd, Config conf) {
    FILE *init = fopen("./resources/init", "r");
    if(init == NULL) {
        fprintf(stderr, "[!] Arquivo init não pode ser aberto\n");
        return;
    }
    // Carrega o processo inicial do arquivo init
    TListaInstrucao programa_init;
    liIniciaLista(&programa_init);
    int num_regs = carrega_executavel(&programa_init, init);
    fclose(init);

    // Inicializa a tabela de processos com o processo inicial apenas
    TTabelaProcesso tabela;
    tpIniciaLista(&tabela);
    tpAdicionaProcesso(&tabela, -1, 0, num_regs, programa_init, 0);

    // Inicializa a lista de bloqueados
    ListaBloqueados bloq;
    bloqueados_inicia(&bloq);

    // Escalonador de filas múltiplas
    TfilasPrioridades filas;
    inicializaTodasFilas(&filas);
    enfileiraProcesso(&filas, tpAcessaProcesso(&tabela, 0));

    // Inicializa a CPU
    CPU cpu;
    inicializaCPU(&cpu, &tabela, &bloq, &filas, conf.esc);

    bool ok = true;
    char buf[BUF_MAX];
    while(ok) {
        int n = read(controle_fd, buf, BUF_MAX);
        for(int i = 0; i < n; ++i) {
            switch(buf[i]) {
                case 'M':
                    printf("Saindo...\n");
                    ok = false;
                    break;
                case 'U':
                    executaUnidadeTempo(&cpu, &filas, &bloq);
                    break;
            }
        }
    }
    zeraCPU(&cpu);
    tpLiberaLista(&tabela);
    bloqueados_libera(&bloq);
}

void executaUnidadeTempo(CPU * cpu, PfilasPrioridades filas, ListaBloqueados *listaBloq) {
    // A primeira coisa que deve ser feita é dar um tique de relógio na filas
    // de bloqueados para atualizar seus estados
    bloqueados_tique(listaBloq);

    // Depois que o tique acontece, pode ser que algum processo tenha sido
    // desbloqueado. Se isso acontecer, devemos recuperar o seu ID e reinseri-lo
    // no escalonador (filas multiplas)
    int idProcessoDesbloqueado;
    while((idProcessoDesbloqueado = bloqueados_remove0(listaBloq)) >= 0) {
        // O processo é reinserido no escalonador
        enfileiraProcesso(filas, tpAcessaProcesso(cpu->pTabela, idProcessoDesbloqueado));
    }
    // Caso o id do processo seja -1, nenhum processo foi desbloqueado, e portanto
    // nada será feito
    // Com o processo devidamente carregado, vamos executar a instrução
    executaProximaInstrucao(cpu);
}

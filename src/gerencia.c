#include <stdio.h>
#include <unistd.h>
#include <assert.h>

#include "bloqueados.h"
#include "gerencia.h"
#include "instrucao.h"
#include "tabela.h"
#include "cpu.h"
#include "filasMultiplas.h"
#include "filasID.h"

#define BUF_MAX 64

int executaUnidadeTempo();

// Laço principal da gerência. Recebe ponta de leitura do pipe e número de
// CPUs a ser utilizado na simulação
void gerencia_main(int controle_fd, int num_cpus) {
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
    // PCaverna caverna = (PCaverna) malloc(sizeof(TipoCaverna));
    TfilasPrioridades filas;
    inicializaTodasFilas(&filas);

    // fprintf(stderr, "Teste");
    // Inicializa a tabela de processos com o processo inicial apenas
    TTabelaProcesso tabela;
    tpIniciaLista(&tabela);

    // Inicializa a lista de bloqueados
    ListaBloqueados bloq;
    bloqueados_inicia(&bloq);

    TfilasPrioridades filasMultiplas;
    inicializaTodasFilas(&filasMultiplas);


    // Inicializa a CPU
    CPU cpu;
    inicializaCPU(&cpu, &tabela, &bloq);
    colocaProcesso(cpu.pTabela, &filas, 0, 0);
    // Temporário! A CPU provavelmente deveria carregar o processo inicial
    // sozinha, na função de executar a próxima instrução
    // carregaProcesso(&cpu, id, 5);


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

int executaUnidadeTempo(CPU * cpu, PfilasPrioridades filas, ListaBloqueados* listaBloq){
    // A primeira coisa que deve ser feita é dar um tique de relógio na filas de bloqueados para atualizar seus estados
    bloqueados_tique(listaBloq);

    // Depois que o tique acontece, pode ser que algum processo tenha sido desbloqueado. Se isso acontecer, devemos recuperar o seu ID e reinseri-lo no escalonador (filas multiplas)
    int idProcessoDesbloqueado = bloqueados_remove0(listaBloq);
    if (idProcessoDesbloqueado != -1){
        colocaProcesso(cpu->pTabela, filas, idProcessoDesbloqueado, EST_BLOQUEADO); // o processo é reinserido no escalonador
    } // Caso o id do processo seja -1, nenhum processo foi desbloqueado, e portanto nada será feito

    // Após a gerencia dos bloqueados, partimos para a execução propriamente dita:

    // Com o processo devidamente carregado, vamos executar a instrução
    executaProximaInstrucao(cpu, filas);
}
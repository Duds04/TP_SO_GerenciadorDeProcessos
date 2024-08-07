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

// Laço principal da gerência. Recebe ponta de leitura do pipe e número de
// CPUs a ser utilizado na simulação
void gerencia_main(int controle_fd, int num_cpus) {
    FILE *init = fopen("/entradas/init", "r");
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
    int id[5];
    id[0] = tpAdicionaProcesso(&tabela, -1, 0, 0, num_regs, programa_init, 0);
    id[1] = tpAdicionaProcesso(&tabela, -1, 0, 0, num_regs, programa_init, 0);
    id[2] = tpAdicionaProcesso(&tabela, -1, 0, 0, num_regs, programa_init, 0);
    id[3] = tpAdicionaProcesso(&tabela, -1, 0, 0, num_regs, programa_init, 0);
    id[4] = tpAdicionaProcesso(&tabela, -1, 0, 0, num_regs, programa_init, 0);

    // colocaProcesso(&tabela, &filas, id[0], 0);
    // colocaProcesso(&tabela, &filas, id[1], 0);
    // colocaProcesso(&tabela, &filas, id[2], 0);
    // colocaProcesso(&tabela, &filas, id[3], 0);
    // imprimeFilasMultiplas(&filas);
    // retiraProcesso(&filas);

    // processo_imprime(tpAcessaProcesso(&tabela, id[0]));
    filas.vetorPrioridades[0].primeiro->idProcesso = 0;
    filas.vetorPrioridades[1].primeiro->idProcesso = 1;
    filas.vetorPrioridades[2].primeiro->idProcesso = 2;
    filas.vetorPrioridades[3].primeiro->idProcesso = 100;
    for (int i = 0; i < 8; i++){
        processo_dados(tpAcessaProcesso(&tabela, id[0]));
        colocaProcesso(&tabela, &filas, id[3], 0);
        processo_dados(tpAcessaProcesso(&tabela, id[0]));
        imprimeFilasMultiplas(&filas);
        retiraProcesso(&filas);
    }
    imprimeFilasMultiplas(&filas);
    // Inicializa a lista de bloqueados
    ListaBloqueados bloq;
    bloqueados_inicia(&bloq);

    // Inicializa a CPU
    CPU cpu;
    inicializaCPU(&cpu, &tabela, &bloq);
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
                    fprintf(stderr, "Desbloqueando\n");
                    bloqueados_tique(&bloq);
                    executaProximaInstrucao(&cpu, NULL);
                    break;
            }
        }
    }
    zeraCPU(&cpu);
    tpLiberaLista(&tabela);
    bloqueados_libera(&bloq);
}

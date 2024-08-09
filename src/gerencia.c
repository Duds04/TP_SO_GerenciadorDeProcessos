#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include "bloqueados.h"
#include "gerencia.h"
#include "instrucao.h"
#include "tabela.h"
#include "multiplasCPUs.h"
#include "filasMultiplas.h"
#include "escalonamento.h"
#include "robin.h"

#define BUF_MAX 64

void executaUnidadeTempo(Config conf, MultiCPUs* cpus, void *escalonador,
        ListaBloqueados *listaBloq, TTabelaProcesso *tabela);

void processoImpressao(Config conf, MultiCPUs* cpus, void *escalonador,
        ListaBloqueados *bloq, TTabelaProcesso *tabela);

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
    int id = tpAdicionaProcesso(&tabela, -1, 0, num_regs, programa_init, 0);

    // Inicializa a lista de bloqueados
    ListaBloqueados bloq;
    bloqueados_inicia(&bloq);

    // Inicializa o escalonador apropriado
    void *escalonador;
    switch(conf.esc) {
        case ESC_FILAS_MULTIPLAS:
            escalonador = malloc(sizeof(TfilasPrioridades));
            inicializaTodasFilas(escalonador);
            enfileiraProcesso(escalonador, tpAcessaProcesso(&tabela, id));
            break;
        case ESC_ROBIN:
            escalonador = malloc(sizeof(FilaRobin));
            robin_inicia(escalonador);
            robin_adiciona(escalonador, id);
            break;
        default:
            exit(65);
    }

    // Inicializa as CPUs (número dado na config)
    MultiCPUs cpus;
    iniciaMultiCPUs(&cpus, conf.num_cpus, &tabela, &bloq, escalonador, conf.esc);

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
                    executaUnidadeTempo(conf, &cpus, escalonador, &bloq, &tabela);
                    break;
                case 'I':
                    processoImpressao(conf, &cpus, escalonador, &bloq, &tabela);
                    break;
            }
            if(tabela.contadorProcessos == 0) {
                printf("Acabaram os processos em memória, finalizando programa...\n");
                ok = false;
                break;
            }
        }
    }
    liberaMultiCPUs(&cpus);
    tpLiberaLista(&tabela);
    bloqueados_libera(&bloq);

    // Desaloca o escalonador apropriado
    switch(conf.esc) {
        case ESC_FILAS_MULTIPLAS:
            liberaListas(escalonador);
            break;
        case ESC_ROBIN:
            robin_libera(escalonador);
            break;
        default:
            break;
    }
    free(escalonador);
    close(controle_fd);
}

void executaUnidadeTempo(Config conf, MultiCPUs* cpus, void *escalonador,
        ListaBloqueados *listaBloq, TTabelaProcesso *tabela) {
    // A primeira coisa que deve ser feita é dar um tique de relógio na filas
    // de bloqueados para atualizar seus estados
    bloqueados_tique(listaBloq);

    // Depois que o tique acontece, pode ser que algum processo tenha sido
    // desbloqueado. Se isso acontecer, devemos recuperar o seu ID e reinseri-lo
    // no escalonador (filas multiplas)
    int idProcessoDesbloqueado;
    while((idProcessoDesbloqueado = bloqueados_remove0(listaBloq)) >= 0) {
        // O processo é reinserido no escalonador
        esc_adiciona_processo(conf.esc, escalonador, tpAcessaProcesso(tabela,
                    idProcessoDesbloqueado));
    }
    executaProximaInstrucaoMulti(cpus);
}

void processoImpressao(Config conf, MultiCPUs *cpus, void *escalonador, ListaBloqueados *bloq,
        TTabelaProcesso *tabela) {

    printf("\t\tImprimindo dados da(s) CPU(s):\n");
    imprimeMultiCPUs(cpus);

    printf("\t\tImprimindo dados do escalonador:\n");
    esc_imprime_escalonador(conf.esc, escalonador);

    printf("\t\tImprimindo dados da tabela de processos:\n");
    tpImprimeLista(tabela);

    printf("\t\tImprimindo dados da lista de bloqueados:\n");
    bloqueados_imprime(bloq);
}

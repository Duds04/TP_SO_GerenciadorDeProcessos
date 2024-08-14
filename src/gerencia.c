#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "bloqueados.h"
#include "gerencia.h"
#include "instrucao.h"
#include "tabela.h"
#include "computador.h"
#include "filasMultiplas.h"
#include "escalonamento.h"
#include "fila.h"

#define BUF_MAX 64

void executaUnidadeTempo(Config conf, Computador *c, void *escalonador,
        ListaBloqueados *listaBloq, TabelaProcessos *tabela);

void processoImpressao(Config conf, Computador *c, void *escalonador,
        ListaBloqueados *bloq, TabelaProcessos *tabela);

// Laço principal da gerência. Recebe ponta de leitura do pipe e configuração
void gerencia(int controle_fd, Config conf) {

    // No processo de controle o tratador do sinal encerra o processo porem
    // aqui no processo de grrencia queremos que o tratamento volte ao normal
    // (o filho herda tudo do pai inclusive o tratador)
    signal(SIGCHLD, SIG_DFL);

    FILE *init = fopen("./init", "r");
    if(init == NULL) {
        fprintf(stderr, "[!] Arquivo init não pôde ser aberto\n");
        return;
    }
    // Carrega o processo inicial do arquivo init
    Programa programaInit;
    programaInicia(&programaInit);
    int numRegs = programaCarrega(&programaInit, init);
    fclose(init);

    // Inicializa a tabela de processos com o processo inicial apenas
    TabelaProcessos tabela;
    tabelaProcessosInicia(&tabela);
    int id = tabelaProcessosAdiciona(&tabela, -1, 0, numRegs, programaInit, 0);

    // Inicializa a lista de bloqueados
    ListaBloqueados bloq;
    bloqueadosInicia(&bloq);

    // Inicializa o escalonador apropriado
    void *escalonador;
    switch(conf.esc) {
        case ESC_FILAS_MULTIPLAS:
            escalonador = malloc(sizeof(FilasMultiplas));
            filasMultiplasInicia(escalonador);
            filasMultiplasAdiciona(escalonador, tabelaProcessosAcessa(&tabela, id));
            break;
        case ESC_ROBIN:
            escalonador = malloc(sizeof(FilaID));
            filaInicia(escalonador);
            filaAdiciona(escalonador, id);
            break;
        default:
            exit(65);
    }

    // Inicializa as CPUs (número dado na config)
    Computador cpus;
    computadorInicia(&cpus, conf.num_cpus, &tabela, &bloq, escalonador, conf.esc);

    pid_t pi;
    bool ok = true;
    char buf[BUF_MAX];
    while(ok) {
        int n = read(controle_fd, buf, BUF_MAX);
        for(int i = 0; i < n; ++i) {
            switch(buf[i]) {
                case 'M':
                    printf("Tempo médio de resposta: %g\n", (cpus.cpus[0].tempo/ (double) tabela.contadorTodosProcessos));
                    printf("Saindo...\n");
                    ok = false;
                    break;
                case 'U':
                    executaUnidadeTempo(conf, &cpus, escalonador, &bloq, &tabela);
                    break;
                case 'I':
                    pi = fork();
                    if(pi < 0) {
                        perror("[!] Falha ao criar o processo de impressão");
                        return;
                    }
                    // Processo filho é responsável pela impressão dos dados, quando ele termina ele morre
                    else if(pi == 0) {
                        processoImpressao(conf, &cpus, escalonador, &bloq, &tabela);
                        exit(0);
                    }
                    // Processo pai espera o filho acabar para continuar
                    else{
                        int status, w;
                        w = wait(&status);
                        if(w == -1){
                            perror("[!] Falha ao esperar o processo de impressão [não existente]");
                        }
                    }

                    break;
            }
            if(tabela.contadorProcessos == 0) {
                printf("Tempo médio de resposta: %g\n", (cpus.cpus[0].tempo/ (double) tabela.contadorTodosProcessos));
                printf("Acabaram os processos em memória, finalizando programa...\n");
                ok = false;
                break;
            }
        }
    }
    computadorLibera(&cpus);
    tabelaProcessosLibera(&tabela);
    bloqueadosLibera(&bloq);

    // Desaloca o escalonador apropriado
    switch(conf.esc) {
        case ESC_FILAS_MULTIPLAS:
            filasMultiplasImprime(escalonador);
            break;
        case ESC_ROBIN:
            filaLibera(escalonador);
            break;
        default:
            break;
    }
    free(escalonador);
    close(controle_fd);
}

void executaUnidadeTempo(Config conf, Computador *c, void *escalonador,
        ListaBloqueados *listaBloq, TabelaProcessos *tabela) {
    // A primeira coisa que deve ser feita é dar um tique de relógio na filas
    // de bloqueados para atualizar seus estados
    bloqueadosTique(listaBloq);

    // Depois que o tique acontece, pode ser que algum processo tenha sido
    // desbloqueado. Se isso acontecer, devemos recuperar o seu ID e reinseri-lo
    // no escalonador (filas multiplas)
    int idProcessoDesbloqueado;
    while((idProcessoDesbloqueado = bloqueadosRemove0(listaBloq)) >= 0) {
        // O processo é reinserido no escalonador
        escalonamentoAdiciona(conf.esc, escalonador, tabelaProcessosAcessa(tabela,
                    idProcessoDesbloqueado));
    }
    computadorExecuta(c);
}

void processoImpressao(Config conf, Computador *cpus, void *escalonador, ListaBloqueados *bloq,
        TabelaProcessos *tabela) {

    printf("\n\tIMPRIMINDO DADOS DA(S) CPU(S):\n");
    computadorImprime(cpus);

    printf("\n\tIMPRIMINDO DADOS DO ESCALONADOR: \n");
    escalonamentoImprime(conf.esc, escalonador);

    printf("\n\tIMPRIMINDO DADOS DA TABELA DE PROCESSOS: \n");
    tabelaProcessosImprime(tabela);

    printf("\n\tIMPRIMINDO DADOS DA LISTA DE BLOQUEADOS:\n");
    bloqueadosImprime(bloq);
}

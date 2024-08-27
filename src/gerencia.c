#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

#include "gerencia.h"
#include "computador.h"
#include "programa.h"

#define BUF_MAX 64

// Cria um novo processo para impressão
static void processoImpressao(const Computador *sis) {
    pid_t pi = fork();
    if(pi < 0) {
        perror("[!] Falha ao criar o processo de impressão");
        exit(1);
    } else if(pi == 0) {
        // Processo filho é responsável pela impressão dos dados; quando ele termina
        // a impressão, ele finaliza imediatamente
        computadorImprime(sis);
        exit(0);
    }
    // Processo pai espera o filho acabar para continuar
    int status;
    if(wait(&status) < 0)
        perror("[!] Falha ao esperar o processo de impressão [não existente]");
}

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
    programaCarrega(&programaInit, init);
    fclose(init);

    // Inicializa o sistema com o processo inicial
    Computador sis;
    computadorInicia(&sis, conf.num_cpus, conf.escId, programaInit);

    bool ok = true;
    char buf[BUF_MAX];
    while(ok) {
        int n = read(controle_fd, buf, BUF_MAX);
        for(int i = 0; i < n; ++i) {
            switch(buf[i]) {
                case 'M':
                    printf("Tempo médio de resposta: %g\n", computadorTempoMedio(&sis));
                    printf("Saindo...\n");
                    ok = false;
                    break;
                case 'U':
                    if(computadorExecuta(&sis) < 0) {
                        printf("Tempo médio de resposta: %g\n", computadorTempoMedio(&sis));
                        printf("Acabaram os processos em memória, finalizando programa...\n");
                        ok = false;
                    }
                    break;
                case 'I':
                    processoImpressao(&sis);
                    break;
            }
        }
    }
    computadorLibera(&sis);
    close(controle_fd);
}

#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include "gerencia.h"
#include "config.h"

#define MAX 64

// Lê a configuração do programa
void config_read(Config *conf);

// Remove espaços em branco no começo e no final da string dada, modificando
// seu conteúdo e retornando um ponteiro para seu novo começo
char *trim(char *s);

int main(int argc, char *argv[]) {
    Config conf;
    config_read(&conf);

    int chan[2];
    if (pipe(chan) < 0) {
        perror("[!] Falha ao criar pipe");
        return 1;
    }
    pid_t p = fork();
    if(p < 0) {
        perror("[!] Falha ao criar o processo de gerência");
        return 1;
    }
    // Processo filho é responsável pelas funções de gerência, e vai ler os
    // dados do usuário indiretamente a partir do pipe criado
    if(p == 0) {
        close(chan[1]);
        gerencia_main(chan[0], conf);
        close(chan[0]);
        return 0;
    }

    // Processo pai é o de controle: ele lê os dados do usuário e escreve-os
    // no pipe para uso na simulação. Os dados podem vir da entrada padrão ou
    // de um arquivo, dependendo do argumento de linha de comando
    close(chan[0]);
    FILE *inp = stdin;
    bool entrada_arquivo = false;
    if(argc >= 2) {
        inp = fopen(argv[1], "r");
        if(inp == NULL) {
            fprintf(stderr, "[!] Impossível ler o arquivo %s\n", argv[1]);
            return 1;
        }
        entrada_arquivo = true;
    }

    char buf[MAX];
    printf("== CONTROLE DE SIMULAÇÃO ==\n");
    while(1) {
        if(!entrada_arquivo) printf("> ");
        if(fgets(buf, MAX, inp) == NULL) break;

        char *entrada = trim(buf);
        if((entrada[0] == 'U' || entrada[0] == 'I' || entrada[0] == 'M')
                && entrada[1] == '\0') {
            // Comando de simulação lido é válido e deve ser escrito no pipe
            write(chan[1], entrada, 1);
            if(entrada[0] == 'M') break; // controle encerra aqui
            continue;
        }
        fprintf(stderr, "[?] Comando '%s' não reconhecido...\n", entrada);
    }
    close(chan[1]);
    if(entrada_arquivo)
        fclose(inp);
    return 0;
}

// Lê a configuração do programa
void config_read(Config *conf) {
    printf("== CONFIGURAÇÃO DE SIMULAÇÃO ==\n");
    printf("Número de CPUs simuladas: ");
    scanf("%d", &conf->num_cpus);
    if(conf->num_cpus <= 0) {
        fprintf(stderr, "[!] Número de CPUs deve ser positivo\n");
        exit(1);
    }

    printf("\nAlgoritmos de escalonamento:\n");
    printf("%d. Round Robin\n", ESC_ROBIN);
    printf("%d. Filas múltiplas\n", ESC_FILAS_MULTIPLAS);
    printf("Selecione a opção desejada entre as acima: ");
    scanf("%d", &conf->esc);
    if(conf->esc < 0 || conf->esc >= NUM_ESCALONAMENTOS) {
        fprintf(stderr, "[!] Algoritmo de escalonamento inválido\n");
        exit(1);
    }
    getchar(); // '\n' sobressalente
    printf("\n");
}

// Remove espaços em branco no começo e no final da string dada, modificando
// seu conteúdo e retornando um ponteiro para seu novo começo
char *trim(char *s) {
    int i = 0, begin;
    while(s[i] != '\0' && isspace(s[i])) ++i;
    if(s[i] == '\0') return &s[i]; // string vazia ou totalmente em branco
    begin = i;

    while(i < MAX && s[i + 1] != '\0') ++i; // chega ao final da string
    // Itera de trás pra frente até o primeiro caractere que não for um
    // espaço em branco, substituindo o espaço logo antes dele por um '\0'
    while(i >= 0 && isspace(s[i])) --i;
    s[i + 1] = '\0';
    return &s[begin];
}

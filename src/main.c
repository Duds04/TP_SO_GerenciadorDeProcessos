#include <stdio.h>
#include <ctype.h>

#include <unistd.h>
#include <sys/types.h>
#include "gerencia.h"

#define MAX 64

// Remove espaços em branco no começo e no final da string dada, modificando
// seu conteúdo e retornando um ponteiro para seu novo começo
char *trim(char *s);

int main() {
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
        gerencia_main(chan[0]);
        close(chan[0]);
        return 0;
    }

    // Processo pai é o de controle: ele lê os dados do usuário e escreve-os
    // no pipe para uso na simulação
    close(chan[0]);
    char buf[MAX];
    printf("== CONTROLE DE SIMULAÇÃO ==\n");
    while(1) {
        printf("> ");
        if(fgets(buf, MAX, stdin) == NULL) break;
        char *entrada = trim(buf);
        fprintf(stderr, "Entrada: %s\n", entrada);
        if((entrada[0] == 'U' || entrada[0] == 'I' || entrada[0] == 'M')
                && entrada[1] == '\0') {
            // Comando de simulação dado é válido e deve ser escrito no pipe
            write(chan[1], entrada, 1);
            if(entrada[0] == 'M') break; // controle encerra aqui
            continue;
        }
        fprintf(stderr, "[?] Não entendi... Tente novamente\n");
    }
    close(chan[1]);
    return 0;
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

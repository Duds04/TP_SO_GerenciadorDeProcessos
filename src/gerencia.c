#include <stdio.h>
#include <unistd.h>
#include "gerencia.h"

#define BUF_MAX 64

// Laço principal da gerência. Recebe ponta de leitura do pipe
void gerencia_main(int controle_fd) {
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
            }
        }
    }
}

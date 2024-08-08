#ifndef GERENCIA_H
#define GERENCIA_H

#include "config.h"

// Laço principal da gerência. Recebe ponta de leitura do pipe e configuração
void gerencia_main(int controle_fd, Config conf);

#endif // GERENCIA_H

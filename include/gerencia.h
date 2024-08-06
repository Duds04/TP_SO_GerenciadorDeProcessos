#ifndef GERENCIA_H
#define GERENCIA_H

// Laço principal da gerência. Recebe ponta de leitura do pipe e número de
// CPUs a ser utilizado na simulação
void gerencia_main(int controle_fd, int num_cpus);

#endif // GERENCIA_H

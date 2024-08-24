#ifndef CONFIG_H
#define CONFIG_H

#include "escalonamento.h"

// Configuração do programa
typedef struct {
    int num_cpus;          // número de CPUs
    EscalonamentoID escId; // algoritmo de escalonamento
} Config;

#endif // CONFIG_H

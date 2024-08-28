#ifndef CONFIG_H
#define CONFIG_H

#include "memoria.h"
#include "escalonamento.h"

// Configuração do programa
typedef struct {
    int numCpus;           // número de CPUs
    EscalonamentoID escId; // algoritmo de escalonamento
    AlocID alocId;         // algoritmo de alocação de memória
} Config;

#endif // CONFIG_H

#ifndef PROCESSO_H
#define PROCESSO_H

#include <stdint.h>
#include "programa.h"

// Estados em que um processo pode estar
typedef enum {
    EST_PRONTO,
    EST_EXECUTANDO,
    EST_BLOQUEADO,
    EST_FINALIZADO, // estado sentinela para processos mortos (temporários)
} ProcessoEstado;

// Informações de um processo na tabela
typedef struct {
    int id, idPai;         // ID do processo e do processo pai
    int pc;                // último valor do contador de programa (tempo de CPU)
    int prioridade;        // prioridade do processo
    int32_t *reg;          // registradores do processo (na memória principal)
    ProcessoEstado estado; // estado do processo
    Programa codigo;       // instruções do processo
    int tempoInicio;       // unidade de tempo de ínicio
} Processo;

// Inicializa um novo processo
void processoInicia(Processo *proc, int id, int idPai, int pc, int prioridade,
        Programa codigo, int tempoInicio, int32_t *reg);

// Impressão resumida dos dados do processo
void processoImprimeResumido(const Processo *proc);

// Imprime os dados de um processo
void processoImprime(const Processo *proc);

// Desaloca a memória associada a um processo
void processoLibera(Processo *proc);

// Subsituir o código de um processo
void processoSubstituiPrograma(Processo *proc, Programa codigo);

#endif // PROCESSO_H

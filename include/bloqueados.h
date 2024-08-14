#ifndef BLOQUEADOS_H
#define BLOQUEADOS_H

// Representação de um processo bloqueado. Cada um possui um ID e uma certa
// quantidade de unidades de tempo durante as quais o processo continuará
// bloqueado
typedef struct {
    int pid, tempoRestante;
} ProcessoBloqueado;

// Estrutura de controle de processos bloqueados (vetor dinâmico)
typedef struct {
    ProcessoBloqueado *processos;
    int tam, capacidade;
} ListaBloqueados;

// Inicializa lista de bloqueados
void bloqueadosInicia(ListaBloqueados *bloq);

// Insere novo processo bloqueado na lista
void bloqueadosAdiciona(ListaBloqueados *bloq, int pid, int tempo_restante);

// Imprime a lista de bloqueados
void bloqueadosImprime(const ListaBloqueados *bloq);

// Decrementa o tempo restante em todos os processos por uma unidade. Todos os
// processos onde o tempo restante torna-se zero (i.e. com desbloqueio pendente)
// são movidos para o fim da lista
void bloqueadosTique(ListaBloqueados *bloq);

// Remove um processo bloqueado com tempo restante igual a zero da lista,
// retornando seu ID. Caso não haja nenhum processo com tempo restante zerado,
// retorna -1. Invoque repetidas vezes para desbloquear todos os processos com
// desbloqueio pendente
int bloqueadosRemove0(ListaBloqueados *bloq);

// Desaloca a memória para a lista de bloqueados
void bloqueadosLibera(ListaBloqueados *bloq);

#endif // BLOQUEADOS_H

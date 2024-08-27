#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "tabela.h"
#include "memoria.h"
#include "processo.h"

// Inicializa a tabela, conectando-a à memória
void tabelaProcessosInicia(TabelaProcessos *tab, Memoria *mem) {
    Processo *listProcess = (Processo *) malloc(TAM_INIT * sizeof(Processo));
    if (listProcess == NULL) {
        fprintf(stderr, "[!] Erro ao alocar memória para a tabela de processos\n");
        exit(1);
    }
    tab->mem = mem;
    tab->processos = listProcess;
    tab->tamanho = TAM_INIT;
    tab->ultimo = 0;
    tab->contadorProcessos = 0;
    tab->contadorTodosProcessos = 0;
}

// Duplica a quantidade de memória alocada para a tabela
static void aumentaTabela(TabelaProcessos *tab) {
    tab->processos = realloc(tab->processos, tab->tamanho * 2 * sizeof(Processo));
    if (tab->processos == NULL) {
        fprintf(stderr, "[!] Erro ao realocar memória para a tabela de processos\n");
        exit(1);
    }
    tab->tamanho = tab->tamanho*2;
}

// Adiciona um processo à tabela, retornando seu ID. O processo herda a
// a prioridade do pai ou tem prioridade 0, caso não tenha pai
int tabelaProcessosAdiciona(TabelaProcessos *tab, int idPai, int pc,
        Programa codigo, int tempoInicio) {
    if(idPai >= tab->ultimo) {
        fprintf(stderr, "[!] ID %d inválido\n", idPai);
        exit(1);
    }
    // Procura para ver se há algum processo finalizado na tabela
    int id = -1;
    for(int i = 0; i < tab->ultimo; ++i) {
        if(tab->processos[i].estado == EST_FINALIZADO) {
            id = i;
            break;
        }
    }
    Processo proc;
    if(id < 0) {
        // Não foi encontrado um processo finalizado; adicionamos o processo
        // no final, checando se há memória suficiente
        if(tab->ultimo + 1 > tab->tamanho)
            aumentaTabela(tab);
        id = tab->ultimo;
        tab->ultimo++;
    }
    int32_t *regs = memoriaAloca(tab->mem, codigo.numRegs);
    int prioridade = (idPai < 0) ? 0 : tab->processos[idPai].prioridade;
    processoInicia(&proc, id, idPai, pc, prioridade, codigo, tempoInicio, regs);
    tab->processos[id] = proc;
    tab->contadorProcessos++;
    tab->contadorTodosProcessos++;
    return id;
}

// Finaliza um processo na tabela
void tabelaProcessoRemove(TabelaProcessos *tab, int id) {
    // ID inválido => erro do programador
    if(id < 0 || id >= tab->ultimo) {
        fprintf(stderr, "[!] Processo de id %d não existe!\n", id);
        return;
    }
    Processo *proc = &tab->processos[id];
    memoriaLibera(tab->mem, proc->codigo.numRegs, proc->reg);
    processoLibera(proc);
    proc->estado = EST_FINALIZADO;
    tab->contadorProcessos--;
}

// Retorna um ponteiro para o processo dado seu ID
Processo *tabelaProcessosAcessa(const TabelaProcessos *tab, int id) {
    if(id < 0 || id >= tab->ultimo) return NULL;
    return &tab->processos[id];
}

void tabelaProcessosImprime(const TabelaProcessos *tab){
    printf("Processos na tabela: %d\n", tab->contadorProcessos);
    printf("Memória alocada para %d processos\n", tab->tamanho);
    printf("====================================\n");
    for(int i = 0; i < tab->ultimo; i++)
        if(tab->processos[i].estado != EST_FINALIZADO){
            processoImprime(&tab->processos[i]);
            printf("====================================\n");
        }
}

void tabelaProcessosLibera(TabelaProcessos *tab){
    for(int i=0; i<tab->ultimo;i++)
        if(tab->processos[i].estado != EST_FINALIZADO)
        processoLibera(&tab->processos[i]);
    free(tab->processos);
}

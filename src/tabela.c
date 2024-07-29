#include <stdio.h>
#include <stdlib.h>

#include "tabela.h"
#include "instrucao.h"

void tpIniciaLista(TTabelaProcesso *pLista){
    Tprocesso *listProcess = (Tprocesso *) malloc(TAM_INIT * sizeof(Tprocesso));
    if (listProcess == NULL) {
        printf("Erro ao alocar memória!");
        exit(1);
    }
    pLista->processos = listProcess;
    pLista->tamanho = TAM_INIT;
    pLista->ultimo = 0;
}

static void tpAumentaTamanhoDaLista(TTabelaProcesso *pLista){
    pLista->processos = realloc(pLista->processos, pLista->tamanho*2 * sizeof(Tprocesso));
    if (pLista->processos == NULL) {
        printf("Erro ao realocar memória!");
        exit(1);
    }
    pLista->tamanho = pLista->tamanho*2;
}

// Adiciona um processo à tabela
void tpAdicionaProcesso(TTabelaProcesso *pLista, int id_pai, int pc,
        int prioridade, int num_regs, TListaInstrucao codigo, int tempoInicio) {
    // Procura para ver se há algum processo finalizado na tabela
    int id = -1;
    for(int i = 0; i < pLista->ultimo; ++i) {
        if(pLista->processos[i].estado == EST_FINALIZADO) {
            id = i;
            break;
        }
    }
    Tprocesso proc;
    if(id < 0) {
        // Não foi encontrado um processo finalizado; adicionamos o processo
        // no final, checando se há memória suficiente
        if(pLista->ultimo + 1 > pLista->tamanho)
            tpAumentaTamanhoDaLista(pLista);
        id = pLista->ultimo;
        pLista->ultimo++;
    }
    processo_inicia(&proc, id, id_pai, pc, prioridade, num_regs, codigo,
            tempoInicio);
    pLista->processos[id] = proc;
}

// Finaliza um processo na tabela
void tpFinalizaProcesso(TTabelaProcesso *pLista, int id) {
    // ID inválido => erro do programador
    if(id < 0 || id >= pLista->ultimo) {
        fprintf(stderr, "[!] Processo de id %d não existe!\n", id);
        return;
    }
    processo_libera(&pLista->processos[id]);
    pLista->processos[id].estado = EST_FINALIZADO;
}

// Retorna um ponteiro para o processo dado seu ID
Tprocesso *tpAcessaProcesso(const TTabelaProcesso *pLista, int id) {
    if(id < 0 || id >= pLista->ultimo) return NULL;
    return &pLista->processos[id];
}

void tpImprimeLista(TTabelaProcesso *pLista){
    printf("Processo na tabela: %d\n", pLista->ultimo);
    printf("Memória alocada para %d processos\n", pLista->tamanho);
    printf("---------------------------\n");
    for(int i = 0; i < pLista->ultimo; i++)
        processo_imprime(&pLista->processos[i]);
}

void tpLiberaLista(TTabelaProcesso *pLista){
    for(int i=0; i<pLista->ultimo;i++){
        processo_libera(&pLista->processos[i]);
    }
    free(pLista->processos);
}

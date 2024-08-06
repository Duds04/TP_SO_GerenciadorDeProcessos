#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

// Inicializa processo
void processo_inicia(Tprocesso *proc, int id, int id_pai, int pc, int prioridade,
        int num_regs, TListaInstrucao codigo, int tempoInicio) {
    proc->id = id;
    proc->id_pai = id_pai;
    proc->pc = pc;
    proc->prioridade = prioridade;
    proc->estado = EST_PRONTO; // começa no estado pronto
    proc->codigo = codigo;
    proc->tempoInicio = tempoInicio;

    // Aloca a quantidade de registradores necessários
    proc->reg = (int*) calloc(num_regs, sizeof(int));
    if(proc->reg == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    proc->num_regs = num_regs;
}

// Imprime os dados de um processo
void processo_imprime(const Tprocesso *proc) {
    printf("ID: %03d PAI: %03d PRIORIDADE: %02d PC: %02d\n", proc->id,
            proc->id_pai, proc->prioridade, proc->pc);
    printf("ESTADO: ");
    switch(proc->estado) {
        case EST_EXECUTANDO:
            printf("executando\n");
            break;
        case EST_BLOQUEADO:
            printf("bloqueado\n");
            break;
        case EST_PRONTO:
            printf("pronto\n");
            break;
        case EST_FINALIZADO:
            printf("finalizado\n");
            break;
    }

    printf("REGISTRADORES:\n-----------------\n");
    for(int r = 0; r < proc->num_regs; ++r)
        printf("r%d: %d\n", r, proc->reg[r]);
    printf("-----------------\n");

    printf("INSTRUÇÕES:\n-----------------\n");
    liImprimeLista(&proc->codigo, proc->pc);
    printf("-----------------\n");
    printf("Tempo de início: %d\n", proc->tempoInicio);
}

// Desaloca a memória associada a um processo
void processo_libera(Tprocesso *proc) {
    liLiberaLista(&proc->codigo);
    free(proc->reg);
}

void substituiPrograma(Tprocesso *proc, TListaInstrucao codigo, int num_regs) {
    processo_libera(&proc->codigo);
    proc->codigo = codigo;
    proc->pc = 0;

    // Aloca a quantidade de registradores necessários
    proc->reg = (int*) calloc(num_regs, sizeof(int));
    if(proc->reg == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    proc->num_regs = num_regs;

}


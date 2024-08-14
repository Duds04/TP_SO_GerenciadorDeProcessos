#include <stdio.h>
#include <stdlib.h>
#include "processo.h"

// Inicializa processo
void processoInicia(Processo *proc, int id, int idPai, int pc, int prioridade,
        int numRegs, Programa codigo, int tempoInicio) {
    proc->id = id;
    proc->idPai = idPai;
    proc->pc = pc;
    proc->prioridade = prioridade;
    proc->estado = EST_PRONTO; // começa no estado pronto
    proc->codigo = codigo;
    proc->tempoInicio = tempoInicio;

    // Aloca a quantidade de registradores necessários
    proc->reg = (int*) calloc(numRegs, sizeof(int));
    if(proc->reg == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    proc->numRegs = numRegs;
}

// Impressão resumida dos dados do processo
void processoImprimeResumido(const Processo *proc) {
    printf("ID: %03d PAI: %03d PRIORIDADE: %02d PC: %02d\n", proc->id,
            proc->idPai, proc->prioridade, proc->pc);
}

// Imprime os dados de um processo
void processoImprime(const Processo *proc) {
    printf("ID: %03d PAI: %03d PRIORIDADE: %02d PC: %02d\n", proc->id,
            proc->idPai, proc->prioridade, proc->pc);
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
    for(int r = 0; r < proc->numRegs; ++r)
        printf("r%d: %d\n", r, proc->reg[r]);
    printf("-----------------\n");

    printf("INSTRUÇÕES:\n-----------------\n");
    programaImprime(&proc->codigo, proc->pc);
    printf("-----------------\n");
    printf("Tempo de início: %d\n", proc->tempoInicio);
}

// Desaloca a memória associada a um processo
void processoLibera(Processo *proc) {
    programaLibera(&proc->codigo);
    free(proc->reg);
}

void processoSubstituiPrograma(Processo *proc, Programa codigo, int numRegs) {
    processoLibera(proc);
    proc->codigo = codigo;
    proc->pc = 0;

    // Aloca a quantidade de registradores necessários
    proc->reg = (int*) calloc(numRegs, sizeof(int));
    if(proc->reg == NULL) {
        fprintf(stderr, "[!] Sem memória suficiente\n");
        exit(64);
    }
    proc->numRegs = numRegs;

}


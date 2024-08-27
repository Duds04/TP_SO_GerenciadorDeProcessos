#include <stdio.h>
#include <stdlib.h>

#include "programa.h"
#include "processo.h"

// Inicializa um novo processo
void processoInicia(Processo *proc, int id, int idPai, int pc, int prioridade,
        Programa codigo, int tempoInicio, int32_t *reg) {
    proc->id = id;
    proc->idPai = idPai;
    proc->pc = pc;
    proc->prioridade = prioridade;
    proc->estado = EST_PRONTO; // começa no estado pronto
    proc->codigo = codigo;
    proc->tempoInicio = tempoInicio;
    proc->reg = reg;
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
    for(int r = 0; r < proc->codigo.numRegs; ++r)
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
}

void processoSubstituiPrograma(Processo *proc, Programa codigo) {
    fprintf(stderr, "[!] Substituição da imagem ainda não foi implementada!\n");
    exit(1024);
}

#include <stdio.h>

#include "memoria.h"
#include "paginas.h"
#include "programa.h"
#include "processo.h"

// Inicializa um novo processo
void processoInicia(Processo *proc, int id, int idPai, int pc, int prioridade,
        int tempoInicio, Programa codigo) {
    proc->id = id;
    proc->idPai = idPai;
    proc->pc = pc;
    proc->prioridade = prioridade;
    proc->estado = EST_PRONTO; // começa no estado pronto
    proc->codigo = codigo;
    proc->tempoInicio = tempoInicio;
    proc->pags = (ProcessoPagInfo) {
        .paginaInicial = -1, // não alocado ainda
        .numPaginas = numPaginasVar(codigo.numRegs),
        .noDisco = false,
    };
}

// Impressão resumida dos dados do processo
void processoImprimeResumido(const Processo *proc) {
    printf("ID: %03d PAI: %03d PRIORIDADE: %02d PC: %02d\n", proc->id,
            proc->idPai, proc->prioridade, proc->pc);
}

// Imprime os dados de um processo
void processoImprime(const Processo *proc, const Memoria *mem) {
    int32_t *reg = memoriaAcessaConst(mem, proc->pags);
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

    if(reg != NULL) {
        printf("REGISTRADORES:\n-----------------\n");
        for(int r = 0; r < proc->codigo.numRegs; ++r)
            printf("r%d: %d\n", r, reg[r]);
        printf("-----------------\n");
    }

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
    processoLibera(proc);
    proc->codigo = codigo;
    proc->pc = 0;
    // Processo perde a referência para suas páginas antigas, que devem
    // ser liberadas externamente a essa função
    proc->pags = PAGINA_NULA;
    proc->pags.numPaginas = numPaginasVar(codigo.numRegs);
}

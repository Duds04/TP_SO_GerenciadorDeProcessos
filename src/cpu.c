#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cpu.h"
#include "instrucao.h"
#include "tabela.h"
#include "escalonamento.h"

// Deixa a CPU zerada
static void zeraCPU(CPU *cpu) {
    cpu->tabela = NULL;
    cpu->listaBloqueados = NULL;
    cpu->esc = (Escalonador){ .id = (-1), .dados = NULL };

    cpu->pidProcessoAtual = -1;
    cpu->pc = 0;
    cpu->quantum = 0;
    cpu->reg = NULL;
    cpu->numRegs = 0;
    cpu->codigo = NULL;
}

// Inicializa a CPU com referências à módulos externos necessários à sua operação
void cpuInicia(CPU *cpu, TabelaProcessos *tab,
        ListaBloqueados *listaBloqueados, Escalonador esc) {
    zeraCPU(cpu);
    cpu->tabela = tab;
    cpu->listaBloqueados = listaBloqueados;
    cpu->esc = esc;
}

// Checa se não há nenhum processo carregado
bool cpuEstaLivre(const CPU *cpu) {
    return cpu->pidProcessoAtual == -1;
}

// Carrega um processo na CPU pelo ID, com quantum fixo
void cpuCarregaProcesso(CPU *cpu, int pidProcessoAtual) {
    Processo* processo = tabelaProcessosAcessa(cpu->tabela, pidProcessoAtual);
    if(processo == NULL) {
        fprintf(stderr, "[!] Processo %d não encontrado\n", pidProcessoAtual);
        exit(1);
    }
    processo->estado = EST_EXECUTANDO;
    cpu->pidProcessoAtual = pidProcessoAtual;
    cpu->pc = processo->pc;
    cpu->quantum = escalonadorQuantum(cpu->esc, processo->prioridade);
    cpu->reg = processo->reg;
    cpu->numRegs = processo->numRegs;
    cpu->codigo = &processo->codigo;
}

// Define o número de variáveis declaradas no processo (em tempo de execução,
static int instrucaoN(CPU *cpu) {
    return cpu->numRegs;
}

// Declaração de variáveis, (informando que existe a variável x)
static void instrucaoD(int x, CPU *cpu) {
    cpu->reg[x] = 0;
}
// Atribuição de valor a variável
static void instrucaoV(int x, int n, CPU *cpu){
    cpu->reg[x] = n;
}
 // Adição de um valor ao valor da variável
static void instrucaoA(int x, int n, CPU *cpu){
    cpu->reg[x] += n;
}
// Subtração de um valor ao valor da variável
static void instrucaoS(int x, int n, CPU *cpu){
     cpu->reg[x] -= n;
}
// Bloqueio de processo por n unidades de tempo
static void instrucaoB(int n, CPU *cpu) {
    bloqueadosAdiciona(cpu->listaBloqueados, cpu->pidProcessoAtual, n);
    tabelaProcessosAcessa(cpu->tabela, cpu->pidProcessoAtual)->estado = EST_BLOQUEADO;
}
// Término de processo
static void instrucaoT(CPU *cpu){
    tabelaProcessoRemove(cpu->tabela, cpu->pidProcessoAtual);
    cpu->pidProcessoAtual = -1;
}

// Cria um novo processo
static void instrucaoF(int n, CPU *cpu, int tempo) {
    Programa novoCodigo;
    programaCopia(cpu->codigo, &novoCodigo);
    int id = tabelaProcessosAdiciona(cpu->tabela, cpu->pidProcessoAtual, cpu->pc + 1,
            novoCodigo, tempo);
    escalonadorAdiciona(cpu->esc, tabelaProcessosAcessa(cpu->tabela, id));
    cpu->pc += n;
}

// Substitui o programa de um processo
static void instrucaoR(const char* nome_do_arquivo, CPU *cpu){
    FILE* arquivo = fopen(nome_do_arquivo, "r");
    if(arquivo == NULL) {
        fprintf(stderr, "[!] Falha ao abrir o arquivo %s\n", nome_do_arquivo);
        exit(1);
    }
    Processo* processo = tabelaProcessosAcessa(cpu->tabela, cpu->pidProcessoAtual);

    Programa novoCodigo;
    programaInicia(&novoCodigo);
    programaCarrega(&novoCodigo, arquivo);
    fclose(arquivo);

    processoSubstituiPrograma(processo, novoCodigo);
    cpu->pc = 0;
}

// Salva os dados do processo atual na tabela e o adiciona no escalonador
static void salvaContexto(CPU *cpu) {
    Processo *proc = tabelaProcessosAcessa(cpu->tabela, cpu->pidProcessoAtual);
    if(proc == NULL) {
        fprintf(stderr, "[!] Processo %d não encontrado\n", cpu->pidProcessoAtual);
        exit(1);
    }
    proc->pc = cpu->pc;
    escalonadorAdiciona(cpu->esc, proc);
}

int cpuExecutaProximaInstrucao(CPU *cpu, int tempo) {
    if(cpuEstaLivre(cpu)) {
        // Se a CPU estiver vazia, carrega um processo do escalonador
        int idProcessoAtual = escalonadorRemove(cpu->esc);
        if (idProcessoAtual < 0) return 1; // sem processos
        cpuCarregaProcesso(cpu, idProcessoAtual);
    } else if(cpu->quantum == 0) {
        // O quantum do processo atual acabou; novamente, um processo deve ser
        // carregado do escalonador
        salvaContexto(cpu);
        int idProcessoAtual = escalonadorRemove(cpu->esc);
        if (idProcessoAtual < 0) {
            fprintf(stderr, "[!] Sem processos no escalonador\n");
            exit(0);
        }
        cpuCarregaProcesso(cpu, idProcessoAtual);
    }
    if(cpu->pc >= cpu->codigo->ultimo) {
        fprintf(stderr, "[!] Processo %d com PC inválido\n", cpu->pidProcessoAtual);
        exit(1);
    }

    Instrucao instrucao = cpu->codigo->intrucoes[cpu->pc];
    printf("Executando instrução ");
    instrucaoImprime(&instrucao);
    printf("\n");
    switch(instrucao.op) {
        case 'N':
            instrucaoN(cpu);
            break;
        case 'D':
            instrucaoD(instrucao.arg0, cpu);
            break;
        case 'V':
            instrucaoV(instrucao.arg0, instrucao.arg1, cpu);
            break;
        case 'A':
            instrucaoA(instrucao.arg0, instrucao.arg1, cpu);
            break;
        case 'S':
            instrucaoS(instrucao.arg0, instrucao.arg1, cpu);
            break;
        case 'B':
            instrucaoB(instrucao.arg0, cpu);
            break;
        case 'T':
            instrucaoT(cpu);
            break;
        case 'F':
            instrucaoF(instrucao.arg0, cpu, tempo);
            break;
        case 'R':
            instrucaoR(instrucao.arq, cpu);
            break;
        default:
            fprintf(stderr, "[!] Instrução inválida\n");

    }

    cpu->pc += 1;
    cpu->quantum -= 1; // cada instrução executada decrementa o quantum
    return 0;
}

void cpuImprime(const CPU *cpu, int numeroCPU, int tempo) {
    printf("TEMPO: %d\nQUANTUM: %d\n", tempo, cpu->quantum);
    printf("ID DO PROCESSO ATUAL: %d\nPC DO PROCESSO ATUAL: %d\n------------------\n", cpu->pidProcessoAtual, cpu->pc);
    printf("\nDADOS DO PROCESSO NA CPU #%d\n------------------\n", numeroCPU+1);
    processoImprime(&cpu->tabela->processos[cpu->pidProcessoAtual]);
    printf("------------------\n\n");
}

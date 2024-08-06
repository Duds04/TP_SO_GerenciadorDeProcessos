#include <stdio.h>
#include <stdlib.h>

#include "cpu.h"

void inicializaCPU(CPU* cpu, TTabelaProcesso *pLista, ListaBloqueados* listaBloqueados)
{
    cpu->pidProcessoAtual = -1;
    cpu->PC = 0;
    cpu->Quantum = 0;
    cpu->pTabela = pLista;
    cpu->listaBloqueados = listaBloqueados;
    cpu->reg = NULL;
    cpu->num_regs = 0;
}


int cpuIsLivre(CPU* cpu)
{
    return cpu->pidProcessoAtual == -1;
}

// reseta a CPU
void zeraCPU(CPU* cpu)
{
    cpu->pidProcessoAtual = -1;
    cpu->PC = 0;
    cpu->Quantum = 0;
    cpu->pTabela = NULL;
    cpu->reg = NULL;
    cpu->num_regs = 0;
    cpu->listaBloqueados = NULL;
}

void carregaProcesso(CPU* cpu, int pidProcessoAtual, int QuantumProcessoAtual)
{
    Tprocesso* processo = tpAcessaProcesso(cpu->pTabela, pidProcessoAtual);
    if(processo == NULL)
    {
        fprintf(stderr, "[!] Processo %d não encontrado\n", pidProcessoAtual);
        exit(1);
    }

    cpu->pidProcessoAtual = pidProcessoAtual;
    cpu->PC = processo->pc;
    cpu->Quantum = QuantumProcessoAtual;
    cpu->reg = processo->reg;
    cpu->num_regs = processo->num_regs;
}

// Executar instruções

// Define o número de variáveis declaradas no processo
static int* instrucaoN(CPU* cpu){
    return cpu->num_regs;
}

// Declaração de variáveis, (informando que existem a variável x)
static void instrucaoD(int x, CPU* cpu){
    cpu->reg[x] = 0;
}

// Atribuição de valor a variável
static void instrucaoV(int x, int n, CPU* cpu){
    cpu->reg[x] = n;
}

 // Adição de um valor ao valor da variável
static void instrucaoA(int x, int n, CPU* cpu){
    cpu->reg[x] =  cpu->reg[x] + n;
}
// Subtração de um valor ao valor da variável
static void instrucaoS(int x, int n, CPU* cpu){
     cpu->reg[x] =  cpu->reg[x] - n;
}
// Bloqueio de processo por n unidades de tempo
static void instrucaoB(int n, CPU* cpu){
    tbBloqueiaProcesso(cpu->listaBloqueados, cpu->pidProcessoAtual, n);
}
// Término de processo
static void instrucaoT(CPU* cpu){
    tpFinalizaProcesso(cpu->pTabela, cpu->pidProcessoAtual);
}

// Cria um novo processo
static void instrucaoF(int n, int pidProcessoAtual, int* pcProcessoAtual, TTabelaProcesso* tabelaProcessos){
    tpCriaProcesso(tabelaProcessos, n, pidProcessoAtual, pcProcessoAtual);
} 
// Substitui o programa de um processo
static void substituiPrograma(const char* nome_do_arquivo, Tprocesso* processo, int* pcProcessoNovo); 


void executaProximaInstrucao(CPU* cpu)
{
    if(cpu->pidProcessoAtual == -1)
    {
        fprintf(stderr, "[!] CPU vazia\n");
        exit(1);
    }

    Tprocesso* processo = tpAcessaProcesso(cpu->pTabela, cpu->pidProcessoAtual);
    if(processo == NULL)
    {
        fprintf(stderr, "[!] Processo %d não encontrado\n", cpu->pidProcessoAtual);
        exit(1);
    }

    if(cpu->Quantum == 0)
    {
        fprintf(stderr, "[!] Processo %d excedeu o quantum\n", cpu->pidProcessoAtual);
        exit(1);
    }

    if(cpu->PC >= processo->codigo.tamanho)
    {
        fprintf(stderr, "[!] Processo %d excedeu o tamanho do programa\n", cpu->pidProcessoAtual);
        exit(1);
    }

    

    Instrucao instrucao = processo->codigo.intrucoes[cpu->PC];
    switch(instrucao.op)
    {
        case 'N':
            instrucaoN(instrucao.arg0);
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
            instrucaoF(instrucao.arg0, cpu->pidProcessoAtual, &cpu->PC, cpu->pTabela);
            break;
        case 'R':
            substituiPrograma(instrucao.arq, processo, &cpu->PC);
            break;
        default:
            fprintf(stderr, "[!] Instrução inválida\n");
    }

    cpu->PC += 1;
    cpu->Quantum++;
}
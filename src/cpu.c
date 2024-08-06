#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cpu.h"
#include "tabela.h"

// Inicializa a CPU com referências à módulos externos necessários à sua operação
void inicializaCPU(CPU *cpu, TTabelaProcesso *pLista, ListaBloqueados* listaBloqueados) {
    zeraCPU(cpu);
    cpu->pTabela = pLista;
    cpu->listaBloqueados = listaBloqueados;
}

// Checa se não há nenhum processo carregado
bool cpuIsLivre(const CPU *cpu) {
    return cpu->pidProcessoAtual == -1;
}

// Deixa a CPU zerada
void zeraCPU(CPU *cpu) {
    cpu->pTabela = NULL;
    cpu->listaBloqueados = NULL;

    cpu->tempo = 0;
    cpu->pidProcessoAtual = -1;
    cpu->pc = 0;
    cpu->quantum = 0;
    cpu->reg = NULL;
    cpu->num_regs = 0;
    cpu->codigo = NULL;
}

// Carrega um processo na CPU pelo ID, com quantum fixo
void carregaProcesso(CPU *cpu, int pidProcessoAtual, int QuantumProcessoAtual) {
    Tprocesso* processo = tpAcessaProcesso(cpu->pTabela, pidProcessoAtual);
    if(processo == NULL) {
        fprintf(stderr, "[!] Processo %d não encontrado\n", pidProcessoAtual);
        exit(1);
    }
    cpu->pidProcessoAtual = pidProcessoAtual;
    cpu->pc = processo->pc;
    cpu->quantum = QuantumProcessoAtual;
    cpu->reg = processo->reg;
    cpu->num_regs = processo->num_regs;
    cpu->codigo = &processo->codigo;
}

// Define o número de variáveis declaradas no processo
// NOTA: essa função não seria invocada na prática, pois a instrução N é
// tratada na leitura dos processos
static int instrucaoN(CPU *cpu) {
    return cpu->num_regs;
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
    cpu->reg[x] =  cpu->reg[x] + n;
}
// Subtração de um valor ao valor da variável
static void instrucaoS(int x, int n, CPU *cpu){
     cpu->reg[x] =  cpu->reg[x] - n;
}
// Bloqueio de processo por n unidades de tempo
static void instrucaoB(int n, CPU *cpu) {
    bloqueados_insere(cpu->listaBloqueados, cpu->pidProcessoAtual, n);
    tpAcessaProcesso(cpu->pTabela, cpu->pidProcessoAtual)->estado = EST_BLOQUEADO;
}
// Término de processo
static void instrucaoT(CPU *cpu){
    tpFinalizaProcesso(cpu->pTabela, cpu->pidProcessoAtual);
    cpu->pidProcessoAtual = -1;
}

// Cria um novo processo
static void instrucaoF(int n, TTabelaProcesso* tabelaProcessos, int prioridade, CPU *cpu) {
    TListaInstrucao novoCodigo;
    liCopiaProfunda(cpu->codigo, &novoCodigo);
    tpAdicionaProcesso(tabelaProcessos, cpu->pidProcessoAtual, cpu->pc + n,
            prioridade, cpu->num_regs, novoCodigo, cpu->tempo);
}

// Substitui o programa de um processo
//static void substituiPrograma(const char* nome_do_arquivo, Tprocesso* processo, int* pcProcessoNovo);


void executaProximaInstrucao(CPU *cpu) {
    if(cpu->pidProcessoAtual == -1) {
        fprintf(stderr, "[!] CPU vazia\n");
        exit(1);
    }
    if(cpu->quantum == 0) {
        fprintf(stderr, "[!] Processo %d excedeu o quantum\n", cpu->pidProcessoAtual);
        exit(1);
    }
    if(cpu->pc >= cpu->codigo->ultimo) {
        fprintf(stderr, "[!] Processo %d excedeu o tamanho do programa\n", cpu->pidProcessoAtual);
        exit(1);
    }

    cpu->tempo += 1;
    int prioridade;
    Instrucao instrucao = cpu->codigo->intrucoes[cpu->pc];
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
            prioridade = tpAcessaProcesso(cpu->pTabela, cpu->pidProcessoAtual)->prioridade;
            instrucaoF(instrucao.arg0, cpu->pTabela, prioridade, cpu);
            break;
        /* case 'R':
            substituiPrograma(instrucao.arq, processo, &cpu->pc);
            break; */
        default:
            fprintf(stderr, "[!] Instrução inválida\n");
    }
    cpu->pc += 1;
    cpu->quantum -= 1;
}

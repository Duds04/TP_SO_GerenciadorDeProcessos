#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cpu.h"
#include "instrucao.h"
#include "tabela.h"
#include "escalonamento.h"

// Inicializa a CPU com referências à módulos externos necessários à sua operação
void inicializaCPU(CPU *cpu, TTabelaProcesso *pLista,
        ListaBloqueados *listaBloqueados, void *escalonador, Escalonamento esc) {
    zeraCPU(cpu);
    cpu->pTabela = pLista;
    cpu->listaBloqueados = listaBloqueados;
    cpu->escalonador = escalonador;
    cpu->esc = esc;
}

// Checa se não há nenhum processo carregado
bool cpuIsLivre(const CPU *cpu) {
    return cpu->pidProcessoAtual == -1;
}

// Deixa a CPU zerada
void zeraCPU(CPU *cpu) {
    cpu->pTabela = NULL;
    cpu->listaBloqueados = NULL;
    cpu->escalonador = NULL;

    cpu->tempo = 0;
    cpu->pidProcessoAtual = -1;
    cpu->pc = 0;
    cpu->quantum = 0;
    cpu->reg = NULL;
    cpu->num_regs = 0;
    cpu->codigo = NULL;
}

// Carrega um processo na CPU pelo ID, com quantum fixo
void carregaProcesso(CPU *cpu, int pidProcessoAtual) {
    Tprocesso* processo = tpAcessaProcesso(cpu->pTabela, pidProcessoAtual);
    if(processo == NULL) {
        fprintf(stderr, "[!] Processo %d não encontrado\n", pidProcessoAtual);
        exit(1);
    }
    processo->estado = EST_EXECUTANDO;
    cpu->pidProcessoAtual = pidProcessoAtual;
    cpu->pc = processo->pc;
    cpu->quantum = esc_tamanho_quantum(cpu->esc, processo->prioridade);
    cpu->reg = processo->reg;
    cpu->num_regs = processo->num_regs;
    cpu->codigo = &processo->codigo;
}

// Define o número de variáveis declaradas no processo (em tempo de execução,
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
    cpu->reg[x] += n;
}
// Subtração de um valor ao valor da variável
static void instrucaoS(int x, int n, CPU *cpu){
     cpu->reg[x] -= n;
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
static void instrucaoF(int n, CPU *cpu) {
    TListaInstrucao novoCodigo;
    liCopiaProfunda(cpu->codigo, &novoCodigo);
    int id = tpAdicionaProcesso(cpu->pTabela, cpu->pidProcessoAtual, cpu->pc + 1,
            cpu->num_regs, novoCodigo, cpu->tempo);
    esc_adiciona_processo(cpu->esc, cpu->escalonador, tpAcessaProcesso(cpu->pTabela, id));
    cpu->pc += n;
}

// Substitui o programa de um processo
static void instrucaoR(const char* nome_do_arquivo, CPU *cpu){
    FILE* arquivo = fopen(nome_do_arquivo, "r");
    if(arquivo == NULL) {
        fprintf(stderr, "[!] Falha ao abrir o arquivo %s\n", nome_do_arquivo);
        exit(1);
    }
    Tprocesso* processo = tpAcessaProcesso(cpu->pTabela, cpu->pidProcessoAtual);

    TListaInstrucao novoCodigo;
    liIniciaLista(&novoCodigo);
    int num_regs = carrega_executavel(&novoCodigo, arquivo);
    fclose(arquivo);

    substituiPrograma(processo, novoCodigo, num_regs);
    cpu->pc = 0;
}

// Salva os dados do processo atual na tabela e o adiciona no escalonador
static void salva_contexto(CPU *cpu) {
    Tprocesso *proc = tpAcessaProcesso(cpu->pTabela, cpu->pidProcessoAtual);
    if(proc == NULL) {
        fprintf(stderr, "[!] Processo %d não encontrado\n", cpu->pidProcessoAtual);
        exit(1);
    }
    proc->pc = cpu->pc;
    esc_adiciona_processo(cpu->esc, cpu->escalonador, proc);
}

int executaProximaInstrucao(CPU *cpu) {
    if(cpuIsLivre(cpu)) {
        // Se a CPU estiver vazia, carrega um processo do escalonador
        int idProcessoAtual = esc_retira_processo(cpu->esc, cpu->escalonador);
        if (idProcessoAtual < 0) return 1; // sem processos
        carregaProcesso(cpu, idProcessoAtual);
    } else if(cpu->quantum == 0) {
        // O quantum do processo atual acabou; novamente, um processo deve ser
        // carregado do escalonador
        salva_contexto(cpu);
        int idProcessoAtual = esc_retira_processo(cpu->esc, cpu->escalonador);
        if (idProcessoAtual < 0) {
            fprintf(stderr, "[!] Sem processos no escalonador\n");
            exit(0);
        }
        carregaProcesso(cpu, idProcessoAtual);
    }
    if(cpu->pc >= cpu->codigo->ultimo) {
        fprintf(stderr, "[!] Processo %d com PC inválido\n", cpu->pidProcessoAtual);
        exit(1);
    }

    cpu->tempo += 1;
    Instrucao instrucao = cpu->codigo->intrucoes[cpu->pc];
    printf("Executando instrução ");
    liImprimeInstrucao(instrucao);
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
            instrucaoF(instrucao.arg0, cpu);
            break;
        case 'R':
            instrucaoR(instrucao.arq, cpu);
            break;
        default:
            fprintf(stderr, "[!] Instrução inválida\n");
        
    }

    cpu->pc += 1;
    cpu->quantum -= 1; // a cada instrução executada decrementa o quantum
    return 0;
}

void imprimeCPU(const CPU *cpu, int numeroCPU){
    printf("TEMPO: %d\nQUANTUM: %d\n", cpu->tempo, cpu->quantum);
    printf("ID DO PROCESSO ATUAL: %d\nPC DO PROCESSO ATUAL: %d\n------------------\n", cpu->pidProcessoAtual, cpu->pc);
    printf("\nDADOS DO PROCESSO NA CPU #%d\n------------------\n", numeroCPU+1);
    processo_imprime(&cpu->pTabela->processos[cpu->pidProcessoAtual]);
    printf("------------------\n\n");
}

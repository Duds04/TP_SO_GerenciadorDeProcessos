#ifndef LOTERIA_H
#define LOTERIA_H

typedef struct{
    // vetor de bilhetes que guarda os ids dos processos 
    int* id_processos;
    int ultimo;
    int capacidade;

} FilaLoteria;

// TODO: onde vai ficar guardado o tempo que o processo foi pra cpu? no próprio processo?
// acho que poderia adicionar uma variavel noprocesso de quantos bilhetes ele tem (ou reaproveitar prioridade)
// definir quantos unidades de tempo de distancia de uso da CPU vamos usar para adicionar ou remover bilhetes (funções estaticas?)

void loteria_inicia(FilaLoteria *l);

void loteria_libera(FilaLoteria *l);

// Retorna o id do processo escolhido
int loteria_retira(FilaLoteria *l);

void loteria_adiciona(FilaLoteria *l, int id_processo);

void loteria_imprime(const FilaLoteria *l);

#endif // LOTERIA_H
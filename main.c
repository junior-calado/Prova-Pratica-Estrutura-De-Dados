#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Definição das estruturas

// Definindo a estrutura Tarefa, que representa uma tarefa a ser realizada.
typedef struct Tarefa {
    char descricao[100];    // Descrição da tarefa.
    int prioridade;         // Prioridade da tarefa (0 - baixa, 1 - alta).
    struct Tarefa* proximo; // Ponteiro para a próxima tarefa na lista.
} Tarefa;

// Definindo a estrutura Fila, que representa uma fila de tarefas gerais.
typedef struct Fila {
    Tarefa* inicio; // Ponteiro para o início da fila.
    Tarefa* fim;    // Ponteiro para o fim da fila.
} Fila;

// Definindo a estrutura Pilha, que representa uma pilha de tarefas de alta prioridade.
typedef struct Pilha {
    Tarefa* topo; // Ponteiro para o topo da pilha.
} Pilha;

// Protótipos das funções

// Função para inicializar uma fila, definindo os ponteiros de início e fim como nulos.
void inicializarFila(Fila* fila);

// Função para inicializar uma pilha, definindo o ponteiro topo como nulo.
void inicializarPilha(Pilha* pilha);

// Função para empilhar uma nova tarefa na pilha de alta prioridade.
void empilhar(Pilha* pilha, char* descricao, int prioridade);

// Função para desempilhar a tarefa no topo da pilha de alta prioridade.
Tarefa desempilhar(Pilha* pilha);

// Função para inserir uma nova tarefa na fila de tarefas gerais ou na pilha de alta prioridade, com base na prioridade.
void inserirNaFila(Fila* fila, Pilha* pilha, char* descricao, int prioridade);

// Função para remover a tarefa no início da fila de tarefas gerais.
Tarefa removerDaFila(Fila* fila);

// Função para processar a próxima tarefa, priorizando a pilha de alta prioridade.
void processarProximaTarefa(Fila* fila, Pilha* pilha);

// Função para imprimir o estado atual do sistema, incluindo fila de tarefas gerais e pilha de alta prioridade.
void imprimirEstado(Fila* fila, Pilha* pilha);

// Função para exibir o menu de opções para o usuário.
void imprimirMenu();

// Implementação das funções

void inicializarFila(Fila* fila) {
    fila->inicio = NULL;
    fila->fim = NULL;
}

void inicializarPilha(Pilha* pilha) {
    pilha->topo = NULL;
}

void empilhar(Pilha* pilha, char* descricao, int prioridade) {
    Tarefa* novaTarefa = (Tarefa*)malloc(sizeof(Tarefa));
    if (novaTarefa == NULL) {
        return;
    }

    strcpy(novaTarefa->descricao, descricao);
    novaTarefa->prioridade = prioridade;

    // A nova tarefa é adicionada no topo da pilha, ajustando os ponteiros.
    novaTarefa->proximo = pilha->topo;
    pilha->topo = novaTarefa;
}

Tarefa desempilhar(Pilha* pilha) {
    Tarefa tarefaDesempilhada;
    tarefaDesempilhada.proximo = NULL;

    if (pilha->topo != NULL) {
        // Remove a tarefa no topo da pilha e libera a memória alocada.
        Tarefa* tarefaADesempilhar = pilha->topo;
        tarefaDesempilhada = *tarefaADesempilhar;

        pilha->topo = pilha->topo->proximo;
        free(tarefaADesempilhar);
    }

    return tarefaDesempilhada;
}

void inserirNaFila(Fila* fila, Pilha* pilha, char* descricao, int prioridade) {
    Tarefa* novaTarefa = (Tarefa*)malloc(sizeof(Tarefa));
    if (novaTarefa == NULL) {
        return;
    }

    strcpy(novaTarefa->descricao, descricao);
    novaTarefa->prioridade = prioridade;
    novaTarefa->proximo = NULL;

    if (prioridade == 1) {
        // Se a prioridade for 1, a tarefa é empilhada na pilha de alta prioridade.
        empilhar(pilha, descricao, prioridade);
    } else {
        if (fila->inicio == NULL) {
            // Se a fila estiver vazia, a nova tarefa se torna o início e o fim da fila.
            fila->inicio = novaTarefa;
            fila->fim = novaTarefa;
        } else {
            // Caso contrário, a nova tarefa é adicionada ao final da fila.
            fila->fim->proximo = novaTarefa;
            fila->fim = novaTarefa;
        }
    }
}

Tarefa removerDaFila(Fila* fila) {
    Tarefa tarefaRemovida;
    tarefaRemovida.proximo = NULL;

    if (fila->inicio != NULL) {
        // Remove a tarefa no início da fila e libera a memória alocada.
        Tarefa* tarefaARemover = fila->inicio;
        tarefaRemovida = *tarefaARemover;

        fila->inicio = fila->inicio->proximo;
        free(tarefaARemover);
    }

    return tarefaRemovida;
}

void processarProximaTarefa(Fila* fila, Pilha* pilha) {
    Tarefa proximaTarefa;

    if (fila->inicio == NULL && pilha->topo == NULL) {
        // Se não houver mais tarefas na fila e na pilha, exibe uma mensagem.
        printf("\n");
        printf("#########################################\n");
        printf("#Não há mais tarefas a serem realizadas.#\n");
        printf("#########################################\n\n");
        return;
    }

    if (pilha->topo != NULL && (fila->inicio == NULL || pilha->topo->prioridade > fila->inicio->prioridade)) {
        // Se a pilha de alta prioridade não estiver vazia e sua prioridade for maior
        // do que a tarefa na fila, a próxima tarefa é desempilhada da pilha.
        proximaTarefa = desempilhar(pilha);
    } else {
        // Caso contrário, a próxima tarefa é removida da fila de tarefas gerais.
        proximaTarefa = removerDaFila(fila);
    }

    // Exibe a tarefa que está sendo realizada.
    printf("############### Realizando a tarefa: %s (Prioridade: %d) ###############\n", proximaTarefa.descricao, proximaTarefa.prioridade);
}

void imprimirEstado(Fila* fila, Pilha* pilha) {
    printf("\n\n###############Estado atual:###############\n\n");

  
    if (fila->inicio == NULL && pilha->topo == NULL) {
        // Se não houver tarefas na fila e na pilha, exibe uma mensagem.
        printf("##########################\n");
        printf("#Não há tarefas pendentes#\n");
        printf("##########################\n");
        return;
    }

    printf("\n############Fila de tarefas gerais:###########\n");
    Tarefa* tarefaAtual = fila->inicio;
    while (tarefaAtual != NULL) {
        // Percorre a fila e exibe as tarefas gerais.
        printf("Descricao: %s, Prioridade: %d\n", tarefaAtual->descricao, tarefaAtual->prioridade);
        tarefaAtual = tarefaAtual->proximo;
    }

    printf("\n############Pilha de tarefas de alta prioridade:##########\n");
    tarefaAtual = pilha->topo;
    while (tarefaAtual != NULL) {
        // Percorre a pilha e exibe as tarefas de alta prioridade.
        printf("Descricao: %s, Prioridade: %d\n", tarefaAtual->descricao, tarefaAtual->prioridade);
        tarefaAtual = tarefaAtual->proximo;
    }
}

void imprimirMenu() {
    printf("\nMenu de Opções:\n");
    printf("1 -> Adicionar Tarefa\n");
    printf("2 -> Processar Próxima Tarefa\n");
    printf("3 -> Imprimir Estado Atual\n");
    printf("4 -> Sair\n");
    printf("Escolha uma opção: ");
}

int main() {
    Fila filaTarefas;
    Pilha pilhaPrioridade;

    inicializarFila(&filaTarefas);
    inicializarPilha(&pilhaPrioridade);

    int opcao;
    char descricao[100];
    int prioridade;

    while (1) {
        imprimirMenu();
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite a descrição da tarefa: ");
                scanf(" %[^\n]", descricao);
                printf("Digite a prioridade da tarefa (0 - baixa, 1 - alta): ");
                scanf("%d", &prioridade);

                inserirNaFila(&filaTarefas, &pilhaPrioridade, descricao, prioridade);
                break;
            case 2:
                processarProximaTarefa(&filaTarefas, &pilhaPrioridade);
                break;
            case 3:
                imprimirEstado(&filaTarefas, &pilhaPrioridade);
                break;
            case 777:
                // Comando para limpar a tela do terminal (Linux/Unix).
                system("clear");
                break;
            case 4:
                return 0;
            default:
                printf("Opção inválida. Por favor, escolha uma opção válida.\n");
        }
    }

    return 0;
}
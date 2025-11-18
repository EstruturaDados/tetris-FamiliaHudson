#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// ============================================================================
// ESTRUTURAS E CONSTANTES
// ============================================================================
typedef struct {
    char tipo;      // 'I', 'O', 'T', 'S', 'Z', 'J', 'L'
    int  id;        // ID sequencial
} Peca;

// Fila Circular – 5 peças futuras
#define TAM_FILA 5
typedef struct {
    Peca dados[TAM_FILA];
    int inicio, fim, tamanho;
} Fila;

// Pilha Linear – 3 peças de reserva
#define TAM_PILHA 3
typedef struct {
    Peca dados[TAM_PILHA];
    int topo;
} Pilha;

// ============================================================================
// VARIÁVEIS GLOBAIS
// ============================================================================
Fila fila;
Pilha pilha;
int proximoID = 1;
const char tipos[] = {'I', 'O', 'T', 'S', 'Z', 'J', 'L'};

// ============================================================================
// PROTÓTIPOS
// ============================================================================
void inicializarFila(Fila *f);
void inicializarPilha(Pilha *p);
Peca gerarPeca();
void enqueue(Fila *f, Peca p);
Peca dequeue(Fila *f);
bool filaCheia(Fila *f);
bool filaVazia(Fila *f);

void push(Pilha *p, Peca p);
Peca pop(Pilha *p);
bool pilhaCheia(Pilha *p);
bool pilhaVazia(Pilha *p);

void mostrarFila(Fila *f);
void mostrarPilha(Pilha *p);
void exibirEstado();
void trocarFrenteComTopo();
void trocarTresPrimeirosComPilha();

// ============================================================================
// MAIN
// ============================================================================
int main() {
    srand(time(NULL));
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    // Preenche fila inicial com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca());
    }

    int opcao;
    do {
        system("clear || cls");
        printf("=== TETRIS STACK - CONTROLE DE PECAS ===\n\n");
        exibirEstado();

        printf("\n1 - Jogar peca (remover da frente)\n");
        printf("2 - Reservar peca (fila -> pilha)\n");
        printf("3 - Usar peca da reserva (pilha)\n");
        printf("4 - Trocar frente da fila com topo da pilha\n");
        printf("5 - Trocar 3 primeiros da fila com a pilha (3x3)\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Jogar peça
                if (filaVazia(&fila)) {
                    printf("\nFila vazia!\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    printf("\nPeca jogada: [%c] ID:%d\n", jogada.tipo, jogada.id);
                    enqueue(&fila, gerarPeca());  // repõe
                }
                break;

            case 2: // Reservar (fila → pilha)
                if (filaVazia(&fila)) {
                    printf("\nFila vazia!\n");
                } else if (pilhaCheia(&pilha)) {
                    printf("\nReserva cheia!\n");
                } else {
                    Peca p = dequeue(&fila);
                    push(&pilha, p);
                    enqueue(&fila, gerarPeca());
                    printf("\nPeca [%c] ID:%d enviada para reserva!\n", p.tipo, p.id);
                }
                break;

            case 3: // Usar da reserva
                if (pilhaVazia(&pilha)) {
                    printf("\nReserva vazia!\n");
                } else {
                    Peca usada = pop(&pilha);
                    printf("\nPeca da reserva usada: [%c] ID:%d\n", usada.tipo, usada.id);
                }
                break;

            case 4: // Trocar frente com topo
                trocarFrenteComTopo();
                break;

            case 5: // Trocar 3 primeiros com pilha
                trocarTresPrimeirosComPilha();
                break;

            case 0:
                printf("\nJogo encerrado. Boa sorte no Tetris!\n");
                break;

            default:
                printf("\nOpcao invalida!\n");
        }

        if (opcao != 0) {
            printf("\nPressione ENTER para continuar...");
            while(getchar() != '\n');
            getchar();
        }

    } while (opcao != 0);

    return 0;
}

// ============================================================================
// IMPLEMENTAÇÃO DAS FUNÇÕES
// ============================================================================
void inicializarFila(Fila *f) {
    f->inicio = f->fim = f->tamanho = 0;
}

void inicializarPilha(Pilha *p) {
    p->topo = -1;
}

Peca gerarPeca() {
    Peca p;
    p.tipo = tipos[rand() % 7];
    p.id = proximoID++;
    return p;
}

void enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return;
    f->dados[f->fim] = p;
    f->fim = (f->fim + 1) % TAM_FILA;
    f->tamanho++;
}

Peca dequeue(Fila *f) {
    if (filaVazia(f)) {
        Peca vazio = {' ', 0};
        return vazio;
    }
    Peca p = f->dados[f->inicio];
    f->inicio = (f->inicio + 1) % TAM_FILA;
    f->tamanho--;
    return p;
}

bool filaCheia(Fila *f)  { return f->tamanho == TAM_FILA; }
bool filaVazia(Fila *f)  { return f->tamanho == 0; }

void push(Pilha *p, Peca peca) {
    if (pilhaCheia(p)) return;
    p->dados[++p->topo] = peca;
}

Peca pop(Pilha *p) {
    if (pilhaVazia(p)) {
        Peca vazio = {' ', 0};
        return vazio;
    }
    return p->dados[p->topo--];
}

bool pilhaCheia(Pilha *p)  { return p->topo == TAM_PILHA - 1; }
bool pilhaVazia(Pilha *p)  { return p->topo == -1; }

// Exibição
void mostrarFila(Fila *f) {
    printf("Fila Futuras → ");
    if (filaVazia(f)) {
        printf("(vazia)");
    } else {
        int i = f->inicio;
        for (int count = 0; count < f->tamanho; count++) {
            printf("[%c:%d] ", f->dados[i].tipo, f->dados[i].id);
            i = (i + 1) % TAM_FILA;
        }
    }
    printf("\n");
}

void mostrarPilha(Pilha *p) {
    printf("Reserva (Pilha) → ");
    if (pilhaVazia(p)) {
        printf("(vazia)");
    } else {
        for (int i = p->topo; i >= 0; i--) {
            printf("[%c:%d] ", p->dados[i].tipo, p->dados[i].id);
            if (i > 0) printf("← ");
        }
    }
    printf("\n");
}

void exibirEstado() {
    mostrarFila(&fila);
    mostrarPilha(&pilha);
}

// Troca frente da fila com topo da pilha
void trocarFrenteComTopo() {
    if (filaVazia(&fila)) {
        printf("\nFila vazia! Nada para trocar.\n");
        return;
    }
    if (pilhaVazia(&pilha)) {
        printf("\nReserva vazia! Nada para trocar.\n");
        return;
    }

    Peca temp = fila.dados[fila.inicio];
    fila.dados[fila.inicio] = pilha.dados[pilha.topo];
    pilha.dados[pilha.top

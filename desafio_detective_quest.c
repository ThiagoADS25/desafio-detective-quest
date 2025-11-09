/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_SIZE 10
#define MAX_NOME 50

// Nó da árvore de salas
typedef struct Sala {
    char nome[MAX_NOME];
    struct Sala* esq;
    struct Sala* dir;
} Sala;

// Nó da árvore de pistas
typedef struct PistaNode {
    char pista[MAX_NOME];
    struct PistaNode* esq;
    struct PistaNode* dir;
} PistaNode;

// Nó da tabela hash
typedef struct HashNode {
    char pista[MAX_NOME];
    char suspeito[MAX_NOME];
    struct HashNode* prox;
} HashNode;

// Variáveis globais
PistaNode* raizPistas = NULL;
HashNode* tabelaHash[HASH_SIZE];
bool pistasJaColetadas[5] = {false};

// Declarações das funções primeiro
Sala* criarSala(char* nome);
PistaNode* inserirPista(PistaNode* raiz, char* pista);
void inserirNaHash(char* pista, char* suspeito);
void explorarSalas(Sala* atual);
void verificarSuspeitoFinal();
void mostrarPistasOrdem(PistaNode* raiz);
char* encontrarSuspeito(char* pista);

// Cria uma sala nova
Sala* criarSala(char* nome) {
    Sala* nova = (Sala*)malloc(sizeof(Sala));
    strcpy(nova->nome, nome);
    nova->esq = NULL;
    nova->dir = NULL;
    return nova;
}

// Bota pista na árvore em ordem
PistaNode* inserirPista(PistaNode* raiz, char* pista) {
    if(raiz == NULL) {
        PistaNode* novo = (PistaNode*)malloc(sizeof(PistaNode));
        strcpy(novo->pista, pista);
        novo->esq = NULL;
        novo->dir = NULL;
        return novo;
    }
    
    int cmp = strcmp(pista, raiz->pista);
    if(cmp < 0) {
        raiz->esq = inserirPista(raiz->esq, pista);
    }
    else if(cmp > 0) {
        raiz->dir = inserirPista(raiz->dir, pista);
    }
    
    return raiz;
}

// Bota na tabela hash - liga pista com suspeito
void inserirNaHash(char* pista, char* suspeito) {
    int index = pista[0] % HASH_SIZE;
    
    HashNode* novo = (HashNode*)malloc(sizeof(HashNode));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabelaHash[index];
    tabelaHash[index] = novo;
}

// Acha suspeito de uma pista
char* encontrarSuspeito(char* pista) {
    int index = pista[0] % HASH_SIZE;
    HashNode* atual = tabelaHash[index];
    
    while(atual != NULL) {
        if(strcmp(atual->pista, pista) == 0) {
            return atual->suspeito;
        }
        atual = atual->prox;
    }
    return NULL;
}

// Explora as salas - anda pela árvore e pega pistas
void explorarSalas(Sala* atual) {
    char op;
    int sair = 0;
    
    while(!sair) {
        printf("\n--- Voce esta na: %s ---\n", atual->nome);
        
        // Vê se tem pista aqui
        char pista[MAX_NOME], suspeito[MAX_NOME];
        int idxPista = -1;
        
        if(strcmp(atual->nome, "Biblioteca") == 0 && !pistasJaColetadas[0]) {
            strcpy(pista, "Livro rasgado");
            strcpy(suspeito, "Professor");
            idxPista = 0;
        }
        else if(strcmp(atual->nome, "Cozinha") == 0 && !pistasJaColetadas[1]) {
            strcpy(pista, "Faca suja");
            strcpy(suspeito, "Cozinheiro");
            idxPista = 1;
        }
        else if(strcmp(atual->nome, "Quarto") == 0 && !pistasJaColetadas[2]) {
            strcpy(pista, "Diario aberto");
            strcpy(suspeito, "Herdeiro");
            idxPista = 2;
        }
        else if(strcmp(atual->nome, "Escritorio") == 0 && !pistasJaColetadas[3]) {
            strcpy(pista, "Documento queimado");
            strcpy(suspeito, "Empresario");
            idxPista = 3;
        }
        else if(strcmp(atual->nome, "Jardim") == 0 && !pistasJaColetadas[4]) {
            strcpy(pista, "Pegada molhada");
            strcpy(suspeito, "Jardineiro");
            idxPista = 4;
        }
        
        // Se achou pista, guarda
        if(idxPista != -1) {
            pistasJaColetadas[idxPista] = true;
            raizPistas = inserirPista(raizPistas, pista);
            inserirNaHash(pista, suspeito);
            printf("*** PISTA: %s ***\n", pista);
        }
        
        // Mostra pra onde pode ir
        printf("\nPra onde vai?\n");
        if(atual->esq) printf("(e) %s\n", atual->esq->nome);
        if(atual->dir) printf("(d) %s\n", atual->dir->nome);
        printf("(s) Sair da exploracao\n");
        printf("Escolha: ");
        scanf(" %c", &op);
        
        if(op == 'e' && atual->esq) {
            atual = atual->esq;
        }
        else if(op == 'd' && atual->dir) {
            atual = atual->dir;
        }
        else if(op == 's') {
            sair = 1; // Sai do loop
            printf("Saindo da exploracao...\n");
        }
        else {
            printf("Nao da pra ir pra la! Tente de novo.\n");
        }
    }
}

// Fase final - acusar alguém
void verificarSuspeitoFinal() {
    if(raizPistas == NULL) {
        printf("Nao tem pista nenhuma!\n");
        return;
    }
    
    printf("\n=== PISTAS QUE VOCE TEM ===\n");
    
    // Conta quantas pistas cada suspeito tem
    typedef struct {
        char nome[MAX_NOME];
        int qtd;
    } Contador;
    
    Contador suspeitos[10];
    int numSuspeitos = 0;
    
    // Passa pela hash contando
    for(int i = 0; i < HASH_SIZE; i++) {
        HashNode* atual = tabelaHash[i];
        while(atual != NULL) {
            // Ve se ja conhece esse suspeito
            int j;
            for(j = 0; j < numSuspeitos; j++) {
                if(strcmp(suspeitos[j].nome, atual->suspeito) == 0) {
                    suspeitos[j].qtd++;
                    break;
                }
            }
            // Se nao conhece, bota na lista
            if(j == numSuspeitos) {
                strcpy(suspeitos[numSuspeitos].nome, atual->suspeito);
                suspeitos[numSuspeitos].qtd = 1;
                numSuspeitos++;
            }
            atual = atual->prox;
        }
    }
    
    // Mostra tudo
    printf("\nPista -> Suspeito:\n");
    for(int i = 0; i < HASH_SIZE; i++) {
        HashNode* atual = tabelaHash[i];
        while(atual != NULL) {
            printf("- %s -> %s\n", atual->pista, atual->suspeito);
            atual = atual->prox;
        }
    }
    
    // Pergunta quem é o culpado
    printf("\n=== HORA DE ACUSAR ===\n");
    char acusado[MAX_NOME];
    printf("Quem fez? ");
    scanf("%s", acusado);
    
    // Ve quantas pistas tem contra ele
    int pistasContra = 0;
    for(int i = 0; i < numSuspeitos; i++) {
        if(strcmp(suspeitos[i].nome, acusado) == 0) {
            pistasContra = suspeitos[i].qtd;
            break;
        }
    }
    
    printf("\n=== RESULTADO ===\n");
    if(pistasContra >= 2) {
        printf("CERTO! %d pistas contra %s!\n", pistasContra, acusado);
        printf("Pegamos o bandido!\n");
    } else {
        printf("ERRADO! So %d pista(s) contra %s.\n", pistasContra, acusado);
        printf("Precisa de 2 ou mais pistas!\n");
    }
}

// Mostra pistas em ordem
void mostrarPistasOrdem(PistaNode* raiz) {
    if(raiz != NULL) {
        mostrarPistasOrdem(raiz->esq);
        printf("- %s\n", raiz->pista);
        mostrarPistasOrdem(raiz->dir);
    }
}

int main() {
    // Começa a hash vazia
    for(int i = 0; i < HASH_SIZE; i++) {
        tabelaHash[i] = NULL;
    }
    
    // Monta a mansão
    Sala* entrada = criarSala("Entrada");
    entrada->esq = criarSala("Biblioteca");
    entrada->dir = criarSala("Cozinha");
    entrada->esq->esq = criarSala("Escritorio");
    entrada->dir->dir = criarSala("Quarto");
    entrada->dir->dir->esq = criarSala("Jardim");
    
    int op;
    
    printf("=== JOGO DO DETETIVE ===\n");
    
    do {
        printf("\nMenu:\n");
        printf("1. Explorar casa\n");
        printf("2. Ver pistas\n");
        printf("3. Acusar alguem\n");
        printf("0. Sair do jogo\n");
        printf("Escolha: ");
        scanf("%d", &op);
        
        switch(op) {
            case 1:
                explorarSalas(entrada);
                break;
            case 2:
                printf("\nSuas pistas:\n");
                if(raizPistas == NULL) {
                    printf("Nada ainda\n");
                } else {
                    mostrarPistasOrdem(raizPistas);
                }
                break;
            case 3:
                verificarSuspeitoFinal();
                break;
            case 0:
                printf("Tchau!\n");
                break;
            default:
                printf("Essa opcao nao existe!\n");
        }
    } while(op != 0);
    
    return 0;
}*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

// Criar valores em um determinado intervalo de inteiros
int valorRandom(int max, int min){
    int aux;

    if(max < min){
        aux = max;
        max = min;
        min = aux;
    }

    return  min + (rand() % (max - min + 1));
}

// Estrutura base do Nó dos Mobs
typedef struct node{
    int hp;
    int vigor;
    int defesa;
    int ataque;

    struct node* dir;
    struct node* esq;
} Node;

typedef Node* NodePtr;

// Pilha onde irá conter os Mobs
typedef struct pilha{
    int tmnh;
    NodePtr topo;
} Pilha;

typedef Pilha* PilhaPtr;

// Função de Criação detalhada de Personagem/Mob
NodePtr criar(int hp, int vigor, int defesa, int ataque){
    NodePtr novo = (NodePtr)malloc(sizeof(Node));

    if(novo == NULL){
        puts("Erro em Alocar memória.");
        exit(1);
    }

    novo->hp     = hp;
    novo->vigor  = vigor;
    novo->defesa = defesa;
    novo->ataque = ataque;

    novo->dir = NULL;
    novo->esq = NULL;

    return novo;
}

// Função para criar Personagem/Mob simples
NodePtr criarS(int max, int min){
    NodePtr novo = (NodePtr)malloc(sizeof(Node));

    if(novo == NULL){
        puts("Erro em Alocar memória.");
        exit(1);
    }

    // Configurando status o Personagem/Mob
    novo->vigor  = valorRandom(max, min);
    novo->defesa = valorRandom(max, min);
    novo->hp     = valorRandom(max, min);
    novo->ataque = valorRandom(max, min);

    // Configurando o ponteiro
    novo->dir = NULL;
    novo->esq = NULL;

    return novo;
}

// Função que adiciona o mob a pilha de combate, lembrando os mais forte primeiro, os mais fracos por último
bool addPilha(PilhaPtr pilha, NodePtr perso){
    NodePtr aux = (NodePtr)malloc(sizeof(Node));
    if(aux == NULL){
        puts("ERRO em alocar memória na pilha.");
        return false;
    }

    // Configurando o Novo Nó
    aux->ataque = perso->ataque;
    aux->defesa = perso->defesa;
    aux->hp     = perso->hp;
    aux->vigor  = perso->vigor;

    // Referenciando os nós
    aux->dir = pilha->topo;

    // Atualizando o topo;
    pilha->topo = aux;

    pilha->tmnh++;

    return true;
}

// Remove um item da pilha, necessário para o combate
bool removerItem(PilhaPtr p){
    NodePtr auxNode = (NodePtr)malloc(sizeof(Node));
    if(auxNode == NULL){
        return false;
    }

    auxNode = p->topo;
    p->topo = auxNode->dir;

    free(auxNode);

    p->tmnh--;

    return true;
}

// Mostra a pilha junto a sua altura
bool exibirPilha(PilhaPtr p){
    NodePtr temp = p->topo;

    printf("Topo (%d)\n", p->tmnh);
    printf(" |\n");

    while (temp != NULL) {
        printf("Hp: [%d]\n",     temp->hp);
        printf("Vigor: [%d]\n",  temp->vigor);
        printf("Ataque: [%d]\n", temp->ataque);
        printf("Defesa: [%d]\n", temp->defesa);
        if(temp->dir != NULL) {
            printf(" |\n");
        }
        temp = temp->dir;
    }

    return true;
}

// Teste
int main(){
    puts("Olá mundo!");

    NodePtr persona1 = criarS(10, 1);
    NodePtr persona2 = criarS(10, 1);
    PilhaPtr p = (PilhaPtr)malloc(sizeof(Pilha));

    addPilha(p, persona1);
    addPilha(p, persona2);

    printf("---------- Monstros ----------\n");
    exibirPilha(p);
    printf("------------------------------\n");

    removerItem(p);

    printf("---------- Monstros ----------\n");
    exibirPilha(p);
    printf("------------------------------\n");

    return 0;
}

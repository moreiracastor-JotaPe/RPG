#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct node{
    int hp;
    int vigor;
    int defesa;
    int ataque;
    
    struct node* dir;
    struct node* esq;
} Node;

typedef Node* NodePtr;

typedef struct pilha{
    int tmnh;
    NodePtr topo;
} Pilha;

typedef Pilha* PilhaPtr;

NodePtr criar(int hp, int vigor, int defesa, int ataque){
    NodePtr novo = (NodePtr)malloc(sizeof(Node));
    
    if(novo == NULL){
        puts("Erro em Alocar memória.");
        exit(1);
    }
    
    novo->hp = hp;
    novo->vigor = vigor;
    novo->defesa = defesa;
    novo->ataque = ataque;
    
    novo->dir = NULL;
    novo->esq = NULL;
    
    return novo;
}

bool addPilha(PilhaPtr pilha, NodePtr perso){
    NodePtr aux = (NodePtr)malloc(sizeof(Node));
    if(aux == NULL){
        puts("ERRO em alocar memória na pilha.");
        return false;
    }
    
    // Configurando o Novo Nó
    aux->ataque = perso->ataque;
    aux->defesa = perso->defesa;
    aux->hp = perso->hp;
    aux->vigor = perso->vigor;
    
    // Referenciando os nós
    aux->dir = pilha->topo;
    
    // Atualizando o topo;
    pilha->topo = aux;
    
    pilha->tmnh++;
    
    return true;
}

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

bool exibirPilha(PilhaPtr p){
    NodePtr temp = p->topo;
    
    printf("Topo (%d)\n", p->tmnh);
    printf(" |\n");
    
    while (temp != NULL) {
        printf("Hp: [%d]\n", temp->hp);
        printf("Vigor: [%d]\n", temp->vigor);
        printf("Ataque: [%d]\n", temp->ataque);
        printf("Defesa: [%d]\n", temp->defesa);
        if(temp->dir != NULL) {
            printf(" |\n");
        }
        temp = temp->dir;
    }
    
    return true;
}

int main(){
    puts("Olá mundo!");
    
    NodePtr persona1 = criar(10, 20, 10, 10);
    NodePtr persona2 = criar(30, 10, 15, 2);
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
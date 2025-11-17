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
    
    return true;
}

bool removerItem(PilhaPtr p){
    NodePtr auxNode = (NodePtr)malloc(sizeof(Node));
    if(auxNode == NULL){
        return false;
    }
}

bool exibirPilha(PilhaPtr p){
    if(p->topo == NULL){
        puts("Sem Itens na Pilha.");
        return false;
    }
    
    PilhaPtr auxP = (PilhaPtr)malloc(sizeof(Pilha));
    if(auxP == NULL){
        puts("ERRO em Alocar Memória em Exibir.");
        return false;
    }
    
    return true;
}

int main(){
    puts("Olá mundo!");
    
    return 0;
}
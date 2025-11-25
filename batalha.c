
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// --- STRUCTS DO PERSONAGEM (Mantendo a base anterior) ---
typedef struct {
    char nome[50];
    int vida;
    int vidaMaxima;
    int ataque;
    int defesa;
} Personagem;

// Nó da Lista Duplamente Encadeada Circular
typedef struct NoHeroi {
    Personagem p;
    struct NoHeroi *prox; // Próximo herói na fila
    struct NoHeroi *ant;  // Herói anterior
} NoHeroi;

typedef struct {
    char nome[50];
    int vida;
    int vidaMaxima;
    int ataque;
    int defesa;
} Inimigo;

// --- ESTRUTURA DA PILHA DE AÇÕES ---

// Tipo de ação (1=Ataque, 2=Defesa, 3=Cura)
typedef enum { ATAQUE = 1, DEFESA, CURA } TipoAcao;
//Nó de ações

// Struct para guardar os dados da ação na pilha
typedef struct {
    TipoAcao tipo;
    Personagem *quemFaz; // Quem vai executar essa ação?
} DadosAcao;

typedef struct NoPilha {
    DadosAcao dados;
    struct NoPilha *prox;
} NoPilha;

typedef struct {
    NoPilha *topo;
    int tamanho;
} Pilha;

// --- FUNÇÕES DA PILHA ATUALIZADAS ---

void empilhar(Pilha *p, TipoAcao tipo, Personagem *heroi) {
    NoPilha *novo = (NoPilha*) malloc(sizeof(NoPilha));
    novo->dados.tipo = tipo;
    novo->dados.quemFaz = heroi; // Salva quem é o dono da ação
    novo->prox = p->topo;
    p->topo = novo;
}

// Retorna os dados da ação (tipo e quem faz)
// Retorna uma struct com tipo=-1 se vazia
DadosAcao desempilhar(Pilha *p) {
    DadosAcao d = {(TipoAcao)-1, NULL};
    if (p->topo == NULL) return d;

    NoPilha *temp = p->topo;
    d = temp->dados;

    p->topo = temp->prox;
    free(temp);

    return d;
}

// Adiciona um herói no final da lista circular
void adicionarHeroi(NoHeroi **lista, Personagem p) {
    NoHeroi *novo = (NoHeroi*) malloc(sizeof(NoHeroi));
    novo->p = p;

    if (*lista == NULL) {
        // Lista vazia: ele aponta para si mesmo
        novo->prox = novo;
        novo->ant = novo;
        *lista = novo;
    } else {
        // Insere no "final" (antes do inicio, já que é circular)
        NoHeroi *ultimo = (*lista)->ant;

        novo->prox = *lista;
        novo->ant = ultimo;

        ultimo->prox = novo;
        (*lista)->ant = novo;
    }
}

// Remove um herói específico da roda (caso morra)
// Retorna 1 se a lista ficou vazia (Game Over), 0 caso contrário
int removerHeroi(NoHeroi **lista, NoHeroi *alvo) {
    if (*lista == NULL) return 1;

    // Caso só tenha 1 herói e ele morreu
    if (alvo->prox == alvo) {
        free(alvo);
        *lista = NULL;
        return 1; // Todos morreram
    }

    // Reconecta os vizinhos
    NoHeroi *anterior = alvo->ant;
    NoHeroi *proximo = alvo->prox;

    anterior->prox = proximo;
    proximo->ant = anterior;

    // Se removermos o chefe da fila (onde o ponteiro está), movemos o ponteiro
    if (*lista == alvo) {
        *lista = proximo;
    }

    free(alvo);
    return 0; // Ainda tem gente viva
}

void inicializarPilha(Pilha *p) {
    p->topo = NULL;
    p->tamanho = 0;
}

// --- FUNÇÕES DE BATALHA ---

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int calcularDano(int ataque, int defesa) {
    int base = ataque - (defesa / 2); // Defesa mitiga metade do ataque direto
    if (base < 0) base = 0;
    return base + (rand() % 5);
}

void executarAcao(int tipoAcao, Personagem *p, Inimigo *i) {
    int valor;
    switch(tipoAcao) {
        case ATAQUE:
            valor = calcularDano(p->ataque, i->defesa);
            i->vida -= valor;
            printf(">> [ATAQUE] Voce causou %d de dano em %s!\n", valor, i->nome);
            break;
        case DEFESA:
            // Neste sistema, defesa pode recuperar um pouco de "stamina" ou vida
            valor = 5;
            p->vida += valor;
            if(p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            printf(">> [DEFESA] Voce se concentrou e recuperou %d HP!\n", valor);
            break;
        case CURA:
            valor = 15; // Cura mais forte
            p->vida += valor;
            if(p->vida > p->vidaMaxima) p->vida = p->vidaMaxima;
            printf(">> [POCAO] Voce bebeu uma pocao e curou %d HP!\n", valor);
            break;
    }
}

void batalhaTurnoEquipe(NoHeroi **listaHerois, Inimigo *monstro) {
    Pilha pilhaGlobal;
    pilhaGlobal.topo = NULL;

    int turno = 1;

    printf("!!! BATALHA TÁTICA (1 AÇÃO POR HEROI) !!!\n");

    while (*listaHerois != NULL && monstro->vida > 0) {

        // ==========================================
        // 1. FASE DE ESCOLHA (PREENCHER A PILHA)
        // ==========================================

        limparTela();
        printf("=== TURNO %d: ESCOLHA DE AÇÕES ===\n", turno);

        NoHeroi *atual = *listaHerois;

        // Percorre a lista circular uma vez para cada herói escolher
        if (atual != NULL) {
            do {
                // Pula heróis mortos que ainda não foram removidos da memória
                if(atual->p.vida <= 0) {
                    atual = atual->prox;
                    continue;
                }

                printf("\n>> Vez de %s (HP: %d)\n", atual->p.nome, atual->p.vida);
                printf("Escolha: [1] Atacar | [2] Defender | [3] Curar: ");

                int escolha;
                scanf("%d", &escolha);

                // Validação básica
                if(escolha < 1 || escolha > 3) escolha = 1;
                //  É apenas um empilhar por herói.
                empilhar(&pilhaGlobal, (TipoAcao)escolha, &atual->p);

                atual = atual->prox; // Passa para o próximo herói escolher
            } while (atual != *listaHerois); // Para quando der a volta completa
        }

        // ==========================================
        // 2. FASE DE RESOLUÇÃO (ESVAZIAR A PILHA)
        // ==========================================

        limparTela();
        printf("\n=== RESOLVENDO AÇÕES ===\n");
        // sleep(1); // Opcional: Pausa dramática

        while(pilhaGlobal.topo != NULL) {
            if(monstro->vida <= 0) break; // Se monstro morreu, para.

            // Tira do topo (O último que escolheu age primeiro!)
            DadosAcao acaoAtual = desempilhar(&pilhaGlobal);
            Personagem *agente = acaoAtual.quemFaz;

            // Verifica se o agente ainda está vivo
            if(agente->vida <= 0) {
                printf(">> %s esta desmaiado e nao pode agir!\n", agente->nome);
                continue;
            }

            // Executa a ação
            printf("\n[%s] realiza sua acao...\n", agente->nome);
            executarAcao(acaoAtual.tipo, agente, monstro);

            // --- CONTRA-ATAQUE DO INIMIGO ---
            // Se o inimigo tomou dano (Ataque), ele revida no PRÓXIMO herói da fila
            if (acaoAtual.tipo == ATAQUE && monstro->vida > 0) {

                // Achar o nó do agente na lista para saber quem é o vizinho
                NoHeroi *noAgente = *listaHerois;
                int encontrou = 0;

                if (noAgente != NULL) {
                    do {
                        if (&noAgente->p == agente) {
                            encontrou = 1;
                            break;
                        }
                        noAgente = noAgente->prox;
                    } while (noAgente != *listaHerois);
                }

                if (encontrou) {
                    // O alvo é o PRÓXIMO da lista circular
                    Personagem *alvo = &noAgente->prox->p;

                    printf("   <!> %s REVIDA em %s!\n", monstro->nome, alvo->nome);
                    int dano = calcularDano(monstro->ataque, alvo->defesa);
                    alvo->vida -= dano;
                    printf("   <!> Dano sofrido: %d\n", dano);

                    if(alvo->vida <= 0) {
                        printf("   (X) %s caiu!\n", alvo->nome);
                    }
                }
            }

            // Pausa para ler o que aconteceu
             getchar();
        }

        // ==========================================
        // 3. FIM DO TURNO E CHECAGEM DE DERROTA
        // ==========================================

        // Verifica se todos morreram
        int alguemVivo = 0;
        NoHeroi *check = *listaHerois;
        if(check != NULL) {
            do {
                if(check->p.vida > 0) alguemVivo = 1;
                check = check->prox;
            } while (check != *listaHerois);
        }

        if(!alguemVivo) {
            printf("\nXXX GAME OVER - O GRUPO FOI DIZIMADO XXX\n");
            break;
        }

        if (monstro->vida <= 0) {
            printf("\n*** VITORIA! O INIMIGO FOI DERROTADO! ***\n");
            break;
        }

        printf("\n[Fim do Turno %d] Pressione ENTER para continuar...", turno);
        getchar(); getchar(); // Dois getchars para limpar o buffer e esperar
        turno++;
    }
}

int main() {
    // 1. Inicializa a semente para números aleatórios
    srand(time(NULL));

    printf("=== CONFIGURANDO O RPG ===\n");

    // 2. Criando o Grande Chefe
    Inimigo boss = {"Dragao das Sombras", 400, 400, 35, 10};
    printf("Inimigo criado: %s (HP: %d | ATQ: %d)\n", boss.nome, boss.vida, boss.ataque);

    // 3. Criando a Lista de Heróis (Party)
    NoHeroi *grupo = NULL; // Ponteiro inicial da lista circular

    // Definindo os personagens
    // Ordem estratégica: O Mago ataca, o monstro revida no PRÓXIMO (Paladino).
    Personagem p1_mago = {"Merlin (Mago)", 80, 80, 50, 5};       // Bate muito, apanha muito
    Personagem p2_tank = {"Arthur (Paladino)", 200, 200, 15, 25}; // Aguenta pancada
    Personagem p3_dps  = {"Robin (Ladino)", 120, 120, 30, 10};    // Equilibrado

    // 4. Inserindo na lista circular
    // A ordem de inserção define a ordem na roda
    adicionarHeroi(&grupo, p1_mago);
    adicionarHeroi(&grupo, p2_tank);
    adicionarHeroi(&grupo, p3_dps);

    printf("Grupo formado!\n");
    printf("Ordem na Roda: %s -> %s -> %s -> (volta ao inicio)\n",
           p1_mago.nome, p2_tank.nome, p3_dps.nome);

    printf("\n>>> Pressione ENTER para entrar na masmorra...");
    getchar();

    // 5. Iniciar a Batalha
    // Passamos o endereço de &grupo pois a cabeça da lista pode mudar se o líder morrer
    batalhaTurnoEquipe(&grupo, &boss);

    // 6. Fim de jogo e Limpeza de Memória (Boas práticas)
    printf("\n=== FIM DE JOGO ===\n");

    // Liberação simples da memória (Desalocar a lista circular)
    if (grupo != NULL) {
        NoHeroi *atual = grupo;
        NoHeroi *proxNode;

        // Quebra o ciclo para facilitar o free
        // Encontra o último e aponta para NULL
        NoHeroi *ultimo = grupo->ant;
        ultimo->prox = NULL;

        while (atual != NULL) {
            proxNode = atual->prox;
            free(atual);
            atual = proxNode;
        }
        printf("Memoria liberada com sucesso.\n");
    }

    return 0;
}

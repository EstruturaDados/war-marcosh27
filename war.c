// Bibliotecas necessárias para entrada/saída de dados e manipulação de strings.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 

//STRUCT Territorio agrupa informações relacionadas a um territorio do jogo.
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Declaração das funções
void cadastrarTerritorios(Territorio* mapa, int total); // Função de cadastro de territorios
void exibirTerritorios(const Territorio* mapa, int total); // Função de exibição de territorios
void atacar(Territorio* atacante, Territorio* defensor); // Função de ataque
void liberarMemoria(Territorio* mapa); // Função de liberação de memória

int main (){

    // INICIALIZAÇÃO E ALOCAÇÃO ---------------------------------------------------------
    // Semente para o gerador de números aleatórios.
    srand(time(NULL));

    Territorio* mapa = NULL; // Ponteiro para o nosso vetor de territórios.
    int totalTerritorios = 0;
    int escolhaMenu = -1;
    int indiceAtacante, indiceDefensor;
    
    // ENTRADA DE DADOS -----------------------------------------------------------------

    // Pede ao usuário o número de territórios para o jogo.
    printf("--- Bem-vindo ao WAR ---\n");
    printf("Quantos territorios existirao no mapa? ");
    scanf("%d", &totalTerritorios);

    // Aloca a memória dinamicamente usando calloc.
    // Retorna um ponteiro para a memória alocada e inicializada com zeros.
    mapa = (Territorio*)calloc(totalTerritorios, sizeof(Territorio));

    // Checa se a memória foi alocada com sucesso.
    if (mapa == NULL) {
        printf("Erro: Falha ao alocar memoria para o mapa!\n");
        return 1; // Encerra o programa com um código de erro.
    }

    // Chama a função para cadastro dos territorios
    cadastrarTerritorios(mapa, totalTerritorios);

    // LOOP DO JOGO ---------------------------------------------------------------------

    while (escolhaMenu != 0) {
        exibirTerritorios(mapa, totalTerritorios);
        
        printf("\n--- MENU WAR ---\n");
        printf("[1] Realizar um ataque\n");
        printf("[0] Sair do jogo\n");
        printf("Digite a opcao: ");
        scanf("%d", &escolhaMenu);

        if (escolhaMenu == 1) {
            printf("\n--- Ataque ---\n");
            printf("Digite o numero do territorio ATACANTE: ");
            scanf("%d", &indiceAtacante);
            printf("Digite o numero do territorio DEFENSOR: ");
            scanf("%d", &indiceDefensor);

            // --- VALIDANDO ATAQUE ---
            // Verifica se os territórios envolvidos existem
            if (indiceAtacante < 0 || indiceAtacante >= totalTerritorios ||
                indiceDefensor < 0 || indiceDefensor >= totalTerritorios) {
                printf("\nERRO: Numero de territorio invalido!\n\n");
            
            // Verifica se os territórios envolvidos não são iguais
            } else if (indiceAtacante == indiceDefensor) {
                printf("\nERRO: Um territorio nao pode atacar a si mesmo!\n\n");

            // Verifica se a tropa do territorio atacante é maior que 1
            } else if (mapa[indiceAtacante].tropas <= 1) {
                printf("\nERRO: O territorio atacante precisa ter mais de 1 tropa para atacar!\n\n");

            // Verifica se os territórios não possuem a mesma cor
            } else if (strcmp(mapa[indiceAtacante].cor, mapa[indiceDefensor].cor) == 0) {
                printf("\nERRO: Voce nao pode atacar um territorio da sua propria cor!\n\n");

            } else {
                // Se todas as regras forem satisfeitas, o ataque acontece
                atacar(&mapa[indiceAtacante], &mapa[indiceDefensor]);
            }
        }
    }

    // FIM DO JOGO E LIMPEZA DE MEMÓRIA -------------------------------------------------
    // Libera a memória que foi alocada.
    liberarMemoria(mapa);
    mapa = NULL; // torna o ponteiro nulo

    printf("\nJogo finalizado.\n");
    return 0;
};

// Função de cadastro de territórios, chamada para entrada de dados.
void cadastrarTerritorios(Territorio* mapa, int total) {

    printf("\n--- Cadastro de Territorios ---\n");
    for (int i = 0; i < total; i++) {
        printf("\n--- Territorio %d ---\n", i);

        // Cadastrando o nome do territorio
        printf("Digite o nome do territorio: ");
        scanf(" %[^\n]", mapa[i].nome);

        // Cadastrando a cor do exercito.
        printf("Digite a cor do exercito: ");
        scanf("%s", (mapa + i)->cor);

        // Cadastrando a quantidade de tropas.
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Função de exibição e "print" do estado do mapa com base nos territórios cadastrados.
void exibirTerritorios(const Territorio* mapa, int total) {
    printf("\n\n--- ESTADO ATUAL DO MAPA ---\n");
    printf("========================================\n");
    for (int i = 0; i < total; i++) {
        printf("[%d] Territorio: %s\n", i, mapa[i].nome);
        printf("    Cor: %s, Tropas: %d\n", mapa[i].cor, mapa[i].tropas);
        printf("----------------------------------------\n");
    }
}

// Função de simulção de uma batalha entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {

    // Simula a rolagem de um dado de 6 lados para cada um.
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;

    printf("\n--- Batalha: %s contra %s ---\n", atacante->nome, defensor->nome);
    printf("Dado do ATACANTE (%s): %d\n", atacante->cor, dadoAtaque);
    printf("Dado da DEFESA (%s): %d\n", defensor->cor, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\nVITORIA DO ATACANTE! O territorio %s foi conquistado!\n", defensor->nome);
        // Transfere a cor do exército para o território conquistado.
        strcpy(defensor->cor, atacante->cor);
        // O atacante move metade de suas tropas para o novo território.
        int tropasMovidas = atacante->tropas / 2;
        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;

    } else {
        printf("\nVITORIA DA DEFESA! O ataque foi frustrado!\n");
        // O atacante perde 1 tropa pela derrota na batalha.
        atacante->tropas--;
    }
}

// Libera a memória alocada dinamicamente para o mapa.
void liberarMemoria(Territorio* mapa) {
    printf("\nLiberando memoria alocada...\n");
    free(mapa);
}
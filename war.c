// Bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>

// DECLARAÇÕES --------------------------------------------------------------------------

// Struct para os Territórios
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Struct para os Jogadores, agora com um ponteiro para a missão
typedef struct {
    char cor[10];
    char* missao; // Ponteiro para a missão, alocada dinamicamente
} Jogador;

// Declaraçção de funções
void limparBufferEntrada();
void cadastrarTerritorios(Territorio* mapa, int total, const Jogador jogadores[], int totalJogadores);
void exibirMapa(const Territorio* mapa, int total);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char** destinoMissao, char* missoes[], int totalMissoes);
bool verificarMissao(const Jogador* jogador, const Territorio* mapa, int totalTerritorios);
bool verificarVitoriaTotal(const Territorio* mapa, int total, char* corVencedora);
void liberarMemoria(Territorio** mapa, Jogador jogadores[], int totalJogadores);

int main() {
    srand(time(NULL));

    // PARAMETRIZAÇÃO INICIAL

    // Declaração de missões do game
    char* missoes[] = {
        "CONQUISTAR 4 TERRITORIOS.",
        "ELIMINAR A COR VERMELHO.",
        "ELIMINAR A COR AZUL.",
        "POSSUIR 20 TROPAS NO TOTAL.",
        "CONQUISTAR O TERRITORIO EUROPA."
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Declaração de Jogadores
    Jogador jogadores[2];
    strcpy(jogadores[0].cor, "AZUL");
    strcpy(jogadores[1].cor, "VERMELHO");
    jogadores[0].missao = NULL;
    jogadores[1].missao = NULL;

    printf("--- Bem-vindo ao WAR ---\n\n");

    // Atribui e exibe a missão de cada jogador
    atribuirMissao(&jogadores[0].missao, missoes, totalMissoes);
    atribuirMissao(&jogadores[1].missao, missoes, totalMissoes);
    printf("Jogador %s, sua missao secreta e: %s\n", jogadores[0].cor, jogadores[0].missao);
    printf("Jogador %s, sua missao secreta e: %s\n\n", jogadores[1].cor, jogadores[1].missao);

    // EENTRADA DE DADOS DO MAPA --------------------------------------------------------

    Territorio* mapa = NULL;
    int totalTerritorios = 0;
    do {
        printf("Quantos territorios existirao no mapa? (minimo 4): ");
        scanf("%d", &totalTerritorios);
        limparBufferEntrada(); // chama a função de limpeza de buffer
        if (totalTerritorios < 4) printf("ERRO: O numero de territorios deve ser pelo menos 4.\n"); //Validação da condição de territorios minimos
    } while (totalTerritorios < 4);

    // alocação de memória para o mapa
    mapa = (Territorio*)calloc(totalTerritorios, sizeof(Territorio));
    if (mapa == NULL) {
        printf("ERRO FATAL: Falha ao alocar memoria para o mapa!\n");
        liberarMemoria(&mapa, jogadores, 2);
        return 1;
    }
    cadastrarTerritorios(mapa, totalTerritorios, jogadores, 2);

    // LOOP DO JOGO ---------------------------------------------------------------------
    int turno = 0;
    Jogador* vencedor = NULL;
    char motivoVitoria[100] = "";

    while (vencedor == NULL) {
        exibirMapa(mapa, totalTerritorios);
        
        Jogador* jogadorAtual = &jogadores[turno % 2];
        printf("\n--- TURNO %d: Vez do jogador %s ---\n", turno + 1, jogadorAtual->cor);
        
        int escolhaMenu;
        printf("[1] Realizar um ataque | [2] Passar o turno\nDigite a opcao: ");
        scanf("%d", &escolhaMenu);
        limparBufferEntrada();

        if (escolhaMenu == 1) {
            int indiceAtacante, indiceDefensor;
            printf("\n--- Ataque ---\n");
            printf("Digite o numero do territorio ATACANTE: ");
            scanf("%d", &indiceAtacante);
            limparBufferEntrada();
            printf("Digite o numero do territorio DEFENSOR: ");
            scanf("%d", &indiceDefensor);
            limparBufferEntrada();

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

        // --- Validando Vitória ---

        // A vitoria é valida seja pela conquista total de territorio o o complete total das missões secretas
        char corVencedoraTotal[10];
        if (verificarVitoriaTotal(mapa, totalTerritorios, corVencedoraTotal)) {
            vencedor = (strcmp(jogadores[0].cor, corVencedoraTotal) == 0) ? &jogadores[0] : &jogadores[1];
            strcpy(motivoVitoria, "CONQUISTA TOTAL DE TODOS OS TERRITORIOS!");
        } else {
            for (int i = 0; i < 2; i++) {
                if (verificarMissao(&jogadores[i], mapa, totalTerritorios)) {
                    vencedor = &jogadores[i];
                    snprintf(motivoVitoria, 100, "Missao Secreta Cumprida: \"%s\"", vencedor->missao);
                    break;
                }
            }
        }
        turno++;
    }

    // FIM DO JOGO E LIMPEZA DE MEMÓRIA -------------------------------------------------
    // Libera a memória que foi alocada.
    printf("\n\n================ FIM DE JOGO ================\n");
    exibirMapa(mapa, totalTerritorios);
    printf("O Jogador %s VENCEU O JOGO!\n", vencedor->cor);
    printf("Motivo da Vitoria: %s\n", motivoVitoria);
    printf("=============================================\n");

    liberarMemoria(&mapa, jogadores, 2);
    return 0;
}


// FUNÇÕES ------------------------------------------------------------------------------

// Função de limpeza de buffer de entrada
void limparBufferEntrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função de cadastro de territórios, chamada para entrada de dados.
void cadastrarTerritorios(Territorio* mapa, int total, const Jogador jogadores[], int totalJogadores) {
    printf("\n--- Cadastro de Territorios ---\n");
    for (int i = 0; i < total; i++) {
        printf("\n--- Territorio %d ---\n", i);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        limparBufferEntrada();

        bool corValida = false;
        while(!corValida){
            printf("Cor inicial do exercito (Ex: Azul, Vermelho): ");
            scanf("%s", mapa[i].cor);
            limparBufferEntrada();
            for(int j = 0; mapa[i].cor[j]; j++){
                mapa[i].cor[j] = toupper(mapa[i].cor[j]);
            }
            for (int k = 0; k < totalJogadores; k++){
                if (strcmp(mapa[i].cor, jogadores[k].cor) == 0) {
                    corValida = true;
                    break;
                }
            }
            if (!corValida) printf("ERRO: Cor invalida. Use uma das cores dos jogadores.\n");
        }

        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limparBufferEntrada();
    }
}

// Função de exibição e "print" do estado do mapa com base nos territórios cadastrados.
void exibirMapa(const Territorio* mapa, int total) {
    printf("\n\n--- ESTADO ATUAL DO MAPA ---\n");
    printf("===============================================\n");
    for (int i = 0; i < total; i++) {
        printf("[%d] %-20s | Cor: %-10s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("===============================================\n");
}

// Função de simulção de uma batalha entre dois territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtaque = (rand() % 6) + 1;
    int dadoDefesa = (rand() % 6) + 1;
    printf("\n--- Batalha: %s contra %s ---\n", atacante->nome, defensor->nome);
    printf("Dado do ATACANTE (%s): %d\n", atacante->cor, dadoAtaque);
    printf("Dado da DEFESA (%s): %d\n", defensor->cor, dadoDefesa);

    if (dadoAtaque > dadoDefesa) {
        printf("\nVITORIA DO ATACANTE! O territorio %s foi conquistado!\n", defensor->nome);
        strcpy(defensor->cor, atacante->cor);
        int tropasMovidas = (atacante->tropas > 2) ? (atacante->tropas / 2) : 1;
        defensor->tropas = tropasMovidas;
        atacante->tropas -= tropasMovidas;
    } else {
        printf("\nVITORIA DA DEFESA! O ataque foi repelido!\n");
        if (atacante->tropas > 1) atacante->tropas--;
    }
}

// Função de atribuição por sorteio da missão secreta para os jogadores
void atribuirMissao(char** destinoMissao, char* missoes[], int totalMissoes) {
    int indiceSorteado = rand() % totalMissoes;
    char* missaoSorteada = missoes[indiceSorteado];

    *destinoMissao = (char*)malloc(strlen(missaoSorteada) + 1);
    if (*destinoMissao != NULL) {
        strcpy(*destinoMissao, missaoSorteada);
    } else {
        printf("ERRO FATAL: Nao foi possivel alocar memoria para a missao!\n");
        exit(1);
    }
}

// Função de validação dos avanços das missões
bool verificarMissao(const Jogador* jogador, const Territorio* mapa, int totalTerritorios) {
    if (strcmp(jogador->missao, "CONQUISTAR 4 TERRITORIOS.") == 0) {
        int contagem = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, jogador->cor) == 0) contagem++;
        }
        return contagem >= 4;
    }
    
    if (strcmp(jogador->missao, "ELIMINAR A COR VERMELHO.") == 0) {
        if (strcmp(jogador->cor, "VERMELHO") == 0) return false;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, "VERMELHO") == 0) return false;
        }
        return true;
    }

    if (strcmp(jogador->missao, "ELIMINAR A COR AZUL.") == 0) {
        if (strcmp(jogador->cor, "AZUL") == 0) return false;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, "AZUL") == 0) return false;
        }
        return true;
    }

    if (strcmp(jogador->missao, "POSSUIR 20 TROPAS NO TOTAL.") == 0) {
        int somaTropas = 0;
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].cor, jogador->cor) == 0) somaTropas += mapa[i].tropas;
        }
        return somaTropas >= 20;
    }

    if (strcmp(jogador->missao, "CONQUISTAR O TERRITORIO EUROPA.") == 0) {
        for (int i = 0; i < totalTerritorios; i++) {
            if (strcmp(mapa[i].nome, "EUROPA") == 0) {
                return strcmp(mapa[i].cor, jogador->cor) == 0;
            }
        }
        return false;
    }
    return false;
}

// Função de verificação de vitória
bool verificarVitoriaTotal(const Territorio* mapa, int total, char* corVencedora) {
    if (total <= 0) return false;
    char primeiraCor[10];
    strcpy(primeiraCor, mapa[0].cor);
    for (int i = 1; i < total; i++) {
        if (strcmp(mapa[i].cor, primeiraCor) != 0) return false;
    }
    strcpy(corVencedora, primeiraCor);
    return true;
}
// Libera a memória alocada dinamicamente para o mapa.
void liberarMemoria(Territorio** mapa, Jogador jogadores[], int totalJogadores) {
    printf("\nLiberando toda a memoria alocada...\n");
    if (mapa != NULL && *mapa != NULL) {
        free(*mapa);
        *mapa = NULL;
    }
    for (int i = 0; i < totalJogadores; i++) {
        if (jogadores[i].missao != NULL) {
            free(jogadores[i].missao);
            jogadores[i].missao = NULL;
        }
    }
}
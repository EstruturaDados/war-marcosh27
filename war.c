// Bibliotecas necessárias para entrada/saída de dados e manipulação de strings.
#include <stdio.h>
#include <string.h>

//STRUCT Territorio agrupa informações relacionadas a um territorio do jogo.
struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};

int main (){
    
    // Declaração de array com capacidade para armazenar 5 estruturas do tipo 'Território'
    // cada posição arrya é um território diferente.

    struct Territorio territorios[5];
    int i; // Variável de controle para loops

    printf("--- Cadastro de territorios ---\n\n");
    printf("Por favor, insira os dados dos territorios.\n");

    // ENTRADA DE DADOS -----------------------------------------------------------------

    // Loop 'for' utilizado para repetir o processo de cadastro de territorios 5 vezes.

    for(i = 0; i< 5; i++){
        printf("--- Cadastrando territorio %d ---\n", i+1);
        
        // Cadastrando o nome do territorio
        printf("Digite o nome do territorio: ");
        scanf(" %[^\n]", territorios[i].nome);

        // Cadastrando a cor do exercito.
        printf("Digite a cor do exercito: ");
        scanf("%s", territorios[i].cor);

        // Cadastrando a quantidade de tropas.
        printf("Digite a quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    // SAÍDA DE DADOS -------------------------------------------------------------------

    printf("\n\n --- Listagem de territorios ---\n\n");
    for(i = 0; i < 5; i++){
        printf("Territorio: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
        printf("=====================================================\n");
    }

    return 0;
};
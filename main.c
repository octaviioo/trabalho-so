#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

int main() {
    int opcao;
    int tabuleiroBot[7][7], jogadaBot[7][7], tabuleiroPlayer[7][7], jogadaPlayer[7][7];
    Navio navio[10];
    int pronto = 0, prontoPlayer = 0;
    int cooPlayer[2];

    navio[0].tamanho = 4;
    navio[1].tamanho = 3;
    navio[2].tamanho = 3;
    navio[3].tamanho = 2;
    navio[4].tamanho = 2;
    navio[5].tamanho = 2;
    navio[6].tamanho = 1;
    navio[7].tamanho = 1;
    navio[8].tamanho = 1;
    navio[9].tamanho = 1;

    srand(time(NULL));
    zeraMatriz(tabuleiroBot);
    zeraMatriz(jogadaBot);
    zeraMatriz(tabuleiroPlayer);
    zeraMatriz(jogadaPlayer);

    do {

        limparTela();
        desenhaBarco();
        printf("Escolha uma opcao (1 ou 2, 0 para sair):\n");
        printf("1. Mostrar texto 1\n");
        printf("2. Mostrar texto 2\n");

        printf("Digite sua escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                draw_box_with_text(30, 10, "Bem-vindo ao Menu!"); // Moldura com texto 1
                break;
            case 2:

                limparTela();
                if(!prontoPlayer){
                    insereTabuleiroPlayer(tabuleiroPlayer);
                    prontoPlayer = 1;
                }

                fazJogada(jogadaPlayer, tabuleiroBot, 0);
                imprimeMatriz(jogadaPlayer, 1);

                if(pronto == 0){
                    for (int i = 0; i < 10; i++){
                        geraNavio(tabuleiroBot, &navio[i]);  
                    }
                    pronto = 1;
                }
                
                printf("\n\nTabuleiro de navios:\n");
                imprimeMatriz(tabuleiroBot, 0);

                fazJogada(jogadaBot, tabuleiroPlayer, 1);
                   
                printf("\n\nTabuleiro de jogadas:\n");
                imprimeMatriz(jogadaBot, 1);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }

        if (opcao != 0) {
            
            printf("\nPressione Enter para continuar...");
            getchar(); 
            getchar();  
        }
    } while (opcao != 0);


    return 0;
}
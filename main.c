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

    navioTamanho(navio);

    srand(time(NULL));
    zeraMatriz(tabuleiroBot);
    zeraMatriz(jogadaBot);
    zeraMatriz(tabuleiroPlayer);
    zeraMatriz(jogadaPlayer);

    do {

        limparTela();
        desenhaBarco();
        printf("\n\n");
        printf(" Escolha uma opcao (1 ou 2, 0 para sair):\n");
        printf(" 1 >>> Jogo Modo Classico (SinglePlayer)\n");
        printf(" 2 >>> Jogo Modo Rapido (SinglePlayer)\n");

        printf(" Digite sua escolha: ");
        scanf("%d", &opcao);

        if(opcao < 0 && opcao > 2){
            printf("\nOpcao invalida. Digite novamente...\n");
        }
     } while (opcao < 0 && opcao > 2);

        switch (opcao) {
            case 1:

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

            case 2:

            limparTela();
            if(!prontoPlayer || !pronto){
                    
                prontoPlayer = 1;
                for (int i = 0; i < 10; i++){
                    geraNavio(tabuleiroBot, &navio[i]);  
                    geraNavio(tabuleiroPlayer, &navio[i]); 
                }
                pronto = 1;
                prontoPlayer = 1;
                }

                limparTela();
                printf("Seu tabuleiro gerado: \n");
                imprimeMatriz(tabuleiroPlayer, 0);

                fazJogada(jogadaPlayer, tabuleiroBot, 0);
                imprimeMatriz(jogadaPlayer, 1);
                
                printf("\n\nTabuleiro de navios:\n");
                imprimeMatriz(tabuleiroBot, 0);

                fazJogada(jogadaBot, tabuleiroPlayer, 1);
                   
                printf("\n\nTabuleiro de jogadas:\n");
                imprimeMatriz(jogadaBot, 1);


            break;
            default:
                printf("Opcao invalida! Tente novamente.\n");
        }
    return 0;
}
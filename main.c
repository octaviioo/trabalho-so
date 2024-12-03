#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

int main() {
    int opcao;
    int tabuleiroBot[7][7], jogadaBot[7][7], tabuleiroPlayer[7][7], jogadaPlayer[7][7];
    Navio navio[10], navioPlayer[10];
    int pronto = 0, prontoPlayer = 0, validaPlayer = 0;

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

                if(!prontoPlayer){

                    printf("Monte seu Tabuleiro de navios:\n\n");
                    printf("Tabela de navios disponiveis:\n");
                    printf("Quantidade || Tamanho\n");
                    printf("   1       ||    4\n");
                    printf("   2       ||    3\n");
                    printf("   3       ||    2\n");
                    printf("   4       ||    1\n");



                    for (int i = 0; i < 10; i++){
                        printf("\ni: %d\n", i);
                        if(i == 0){
                            navioPlayer[i].tamanho = 4;
                        }else if(i == 1 || i == 2){
                            navioPlayer[i].tamanho = 3;
                        }else if(i >= 3 && i <= 5){
                            navioPlayer[i].tamanho = 2;
                        }else{
                            navioPlayer[i].tamanho = 1;
                        }
                        
                        printf("\n%d\n", navioPlayer[i].tamanho);

                        do{
                            
                            printf("\nSeu tabuleiro atual:\n");
                            imprimeMatriz(tabuleiroPlayer, 0);

                            printf("\nInsira o navio %d\n", i);

                            printf("Digite o valor da posição x: ");
                            scanf("%d", &navioPlayer[i].x);
                        
                            printf("Digite o valor da posição y: ");
                            scanf("%d", &navioPlayer[i].y);

                            do{
                                printf("Digite a orientacao do navio\n0 para vertical e 1 para horizontal: ");
                                scanf("%d", &navioPlayer[i].direcao);
                                if(navioPlayer[i].direcao < 0 || navioPlayer[i].direcao > 1){
                                    printf("\nPosicao Invalida. Digite novamente...\n");
                                }
                            } while (navioPlayer[i].direcao < 0 || navioPlayer[i].direcao > 1);

                            validaPlayer = verificaPosicaoValida(tabuleiroPlayer, navioPlayer[i]);

                            printf("\n%d\n", validaPlayer);

                            if(!validaPlayer){
                                 printf("\nposicao invalida, digite novamente...\n");
                            }else{
                                insereNavioPlayer(tabuleiroPlayer, navioPlayer[i]);
                            }
                            

                        }while(!validaPlayer);
                    }
                    
                    
                    prontoPlayer = 1;

                }

                if(pronto == 0){
                    for (int i = 0; i < 10; i++){
                        geraNavio(tabuleiroBot, &navio[i]);  
                    }
                    pronto = 1;
                }
                
                printf("\n\nTabuleiro de navios:\n");
                imprimeMatriz(tabuleiroBot, 0);

                fazJogada(jogadaBot, tabuleiroPlayer);
                   
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



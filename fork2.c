#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "funcoes.h"
#include <pthread.h>


#define TEMPO_LIMITE 10

int jogadaFeita = 0;
int tempoTotal = 0;
int continuarContando = 1; 

void *contadorTempoTotal(void *arg) {
    while (continuarContando) {
        sleep(1);
        tempoTotal++;
    }
    pthread_exit(NULL);
}
void client(int readfd, int writefd, int opcao);
void server(int readfd, int writefd);

int main() {
    int descritor, 
        pipe1[2],  
        pipe2[2];  

    int opcao;

    do {
        desenhaBarco();
        printf("\n\n");
        printf(" Escolha uma opcao (1 ou 2, 0 para sair):\n\n");
        printf(" 1 >>> Jogo Modo Classico\n Tabuleiro montado manualmente\n\n");
        printf(" 2 >>> Jogo Modo Rapido\n Tabuleiro gerado aleatoriamente\n\n");

        printf(" Digite sua escolha: ");
        scanf("%d", &opcao);

        if(opcao < 0 || opcao > 2) {
            printf("\nOpcao invalida. Digite novamente...\n");
        }
    } while (opcao < 0 || opcao > 2);

    srand(time(NULL) + getpid());

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("Erro na chamada PIPE");
        exit(0);
    }

    if ((descritor = fork()) < 0) {
        perror("Erro na chamada FORK");
        exit(0);
    }

    else if (descritor > 0) {
        // processo pai
        close(pipe1[0]); // fecha leitura no pipe1
        close(pipe2[1]); // fecha escrita no pipe2

        client(pipe2[0], pipe1[1], opcao); // Chama CLIENTE no PAI

        close(pipe1[1]); // fecha pipe1
        close(pipe2[0]); // fecha pipe2
        exit(0);
    }
    else {
        // Processo Filho
        close(pipe1[1]); // fecha escrita no pipe1
        close(pipe2[0]); // fecha leitura no pipe2

        server(pipe1[0], pipe2[1]); // Chama SERVIDOR no FILHO

        close(pipe1[0]); // fecha leitura no pipe1
        close(pipe2[1]); // fecha escrita no pipe2
        exit(0);
    }

    return 0;
}


//Função responsavel pelas jogadas de Player
void client(int readfd, int writefd, int opcao) {
    int flag;
    Navio navio[10];
    int prontoPlayer = 0, resultadoAtaqueBot;
    int tabuleiroPlayer[7][7], jogadaPlayer[7][7];
    int cooJogada[2], resultadoAtaque, contaNavioBot = 0, contaNavioPlayer = 0;

    zeraMatriz(tabuleiroPlayer);
    zeraMatriz(jogadaPlayer);



    // Inicia a thread para contar o tempo total da partida
    pthread_t threadTempoTotal;
    pthread_create(&threadTempoTotal, NULL, contadorTempoTotal, NULL);

    while (1) {
        if (!prontoPlayer) {
            if (opcao == 1) {
                insereTabuleiroPlayer(tabuleiroPlayer);
            } else {
                navioTamanho(navio);
                for (int i = 0; i < 10; i++) {
                    geraNavio(tabuleiroPlayer, &navio[i]);
                }
            }
            prontoPlayer = 1;
            limparTela();
            printf("Modo Rapido Selecionado!\n");
            printf("Tabuleiro do jogador:\n");
            imprimeMatriz(tabuleiroPlayer, 0);
        }
        if (contaNavioPlayer == 20) {
            limparTela();
            printf("\nParabéns!!! Você venceu!!\n");
            continuarContando = 0;
            printf("\nTempo total da partida: %d segundos\n", tempoTotal);
            sleep(10);
            break;
        }

        // Solicita jogada do jogador
        fazJogada(cooJogada);

        // Envia jogada e flag para o servidor
        write(writefd, cooJogada, sizeof(cooJogada));
        flag = 1;
        write(writefd, &flag, sizeof(flag));

        printf("Enviando ao servidor: Jogada = [%d, %d]\n", cooJogada[0], cooJogada[1]);
        limparTela();

        // Lê o resultado do ataque
        if (read(readfd, &resultadoAtaque, sizeof(resultadoAtaque)) > 0) {
            jogadaPlayer[cooJogada[0]][cooJogada[1]] = resultadoAtaque;
            if(resultadoAtaque == 2){
                contaNavioPlayer++;
                printf("\nNavio atingido!!!\n");
            }else{
                printf("\nAtirou na agua\n");
            }
            printf("Quantidade de navios acertados pelo player: %d/20\n", contaNavioPlayer);
        } else {
            printf("Erro ao ler resultado do ataque.\n");
            continue;
        }

        // Lê a jogada do servidor
        if (read(readfd, cooJogada, sizeof(cooJogada)) > 0 && read(readfd, &flag, sizeof(flag)) > 0) {
            printf("Servidor jogou em: [%d, %d]\n", cooJogada[0], cooJogada[1]);


                if (tabuleiroPlayer[cooJogada[0]][cooJogada[1]] == 1) {
                    printf("\nBot acertou navio\n");
                    resultadoAtaqueBot = 2;
                    contaNavioBot++;
                    tabuleiroPlayer[cooJogada[0]][cooJogada[1]] = 0;
                    if (contaNavioBot == 20) {
                        limparTela();
                        printf(RED "\nGame Over. Bot venceu!!!\n" RESET);
                        continuarContando = 0;
                        printf("\nTempo total da partida: %d segundos\n", tempoTotal);
                        sleep(10);
                        break;
                    }
                } else {
                    printf("\nBot acertou água\n");
                    resultadoAtaqueBot = 1;
                }
                printf("Quantidade de navios acertados pelo bot: %d/20\n", contaNavioBot);
                imprimeDuasMatrizes(tabuleiroPlayer, jogadaPlayer);
           
        } else {
            printf("Erro ao ler jogada do servidor.\n");
        }
    }

    continuarContando = 0;
    pthread_join(threadTempoTotal, NULL);

    printf("\nTempo total da partida: %d segundos\n", tempoTotal);
}

//Função responsavel por jogadas de bot
void server(int readfd, int writefd) {
    int flag;
    int tabuleiroBot[7][7], jogadaBot[7][7];
    int pronto = 0;
    Navio navio[10];
    int ataqueRecebido[2];
    int resultadoAtaque, validaJogada = 0;
    int cooJogada[2]; 

    zeraMatriz(tabuleiroBot);
    zeraMatriz(jogadaBot); 

    while (1) {
 
        printf(RESET);
        int n = read(readfd, ataqueRecebido, sizeof(ataqueRecebido));
        if (n <= 0) {
            printf("Fim de Jogo. Saindo...\n");
            break;
        }

        n = read(readfd, &flag, sizeof(flag));
        if (n <= 0) {
            printf("Erro ao ler flag do cliente ou pipe vazio.\n");
            continue;
        }

        if(!flag){
            printf("Aguardando client...");
            continue;
        }

        if (!pronto) {
            zeraMatriz(tabuleiroBot);
            navioTamanho(navio);
            for (int i = 0; i < 10; i++) {
                geraNavio(tabuleiroBot, &navio[i]);
            }
            pronto = 1;
        }

        printf("Ataque recebido: [%d, %d]\n", ataqueRecebido[0], ataqueRecebido[1]);
        if (tabuleiroBot[ataqueRecebido[0]][ataqueRecebido[1]] == 1) {
            resultadoAtaque = 2; // Navio atingido
            printf("Navio atingido!\n");
        } else {
            resultadoAtaque = 1; // Água
            printf("Nenhum navio atingido.\n");
        }

do {
    cooJogada[0] = rand() % 7; 
    cooJogada[1] = rand() % 7; 

    // Valida se a posição ainda não foi jogada
    if (jogadaBot[cooJogada[0]][cooJogada[1]] == 0) {
        jogadaBot[cooJogada[0]][cooJogada[1]] = 1; 
        validaJogada = 1; 
    } else {
        printf("Jogada repetida. Tentando novamente...\n");
        validaJogada = 0; 
    }
} while (!validaJogada);
         
        flag = 0; 

        write(writefd, &resultadoAtaque, sizeof(resultadoAtaque)); 
        write(writefd, cooJogada, sizeof(cooJogada));           
        write(writefd, &flag, sizeof(flag));                                

        limparTela();
    }
}



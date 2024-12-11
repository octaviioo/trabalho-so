#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "funcoes.h"

#define MAXBUFF 1024 // tamanho do buffer

void jogador(int readfd, int writefd); // Cliente (jogador humano)
void bot(int readfd, int writefd);     // Servidor (bot adversário)

int main() {
    int descritor;        // descritor do fork
    int pipe1[2], pipe2[2]; // pipes para comunicação (pai <-> filho)

    // Criação dos pipes
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("Erro ao criar pipe");
        exit(EXIT_FAILURE);
    }

    // Criação do processo filho
    if ((descritor = fork()) < 0) {
        perror("Erro ao criar fork");
        exit(EXIT_FAILURE);
    }

    if (descritor > 0) {
        // Processo Pai (Jogador)
        close(pipe1[0]); // Fecha leitura no pipe1
        close(pipe2[1]); // Fecha escrita no pipe2

        jogador(pipe2[0], pipe1[1]); // Pai age como cliente
        close(pipe1[1]); // Fecha escrita no pipe1
        close(pipe2[0]); // Fecha leitura no pipe2
    } else {
        // Processo Filho (Bot)
        close(pipe1[1]); // Fecha escrita no pipe1
        close(pipe2[0]); // Fecha leitura no pipe2

        bot(pipe1[0], pipe2[1]); // Filho age como servidor
        close(pipe1[0]); // Fecha leitura no pipe1
        close(pipe2[1]); // Fecha escrita no pipe2
    }

    return 0;
}

// Função para o jogador (cliente)
void jogador(int readfd, int writefd) {
    int tabuleiroPlayer[7][7], jogadaPlayer[7][7], opcao;
    int prontoPlayer = 0;
    char buff[MAXBUFF];

    zeraMatriz(tabuleiroPlayer);
    zeraMatriz(jogadaPlayer);

    while (1) {
        limparTela();
        printf(" Escolha uma opcao (1 ou 2, 0 para sair):\n");
        printf(" 1 >>> Jogo Modo Classico (SinglePlayer)\n");
        printf(" 2 >>> Jogo Modo Rapido (SinglePlayer)\n");
        printf(" Digite sua escolha: ");
        scanf("%d", &opcao);

        if (opcao == 0) {
            printf("Saindo...\n");
            write(writefd, "0", 1); // Envia flag de saída
            break;
        } else if (opcao == 1 || opcao == 2) {
            if (!prontoPlayer) {
                insereTabuleiroPlayer(tabuleiroPlayer);
                prontoPlayer = 1;
            }

            fazJogada(jogadaPlayer, NULL, 0); // Faz jogada do jogador
            snprintf(buff, sizeof(buff), "1,%d,%d", jogadaPlayer[0][0], jogadaPlayer[0][1]);
            write(writefd, buff, strlen(buff)); // Envia jogada e flag "1"

            read(readfd, buff, sizeof(buff)); // Lê resposta do bot
            printf("Resposta do bot: %s\n", buff);
        } else {
            printf("Opcao invalida. Tente novamente.\n");
        }
    }
}

// Função para o bot (servidor)
void bot(int readfd, int writefd) {
    int tabuleiroBot[7][7], jogadaBot[7][7];
    Navio navio[10];
    char buff[MAXBUFF];
    int pronto = 0;

    srand(time(NULL));
    zeraMatriz(tabuleiroBot);
    zeraMatriz(jogadaBot);

    while (1) {
        read(readfd, buff, sizeof(buff)); // Lê jogada do cliente
        if (buff[0] == '0') {
            printf("Jogador saiu do jogo. Encerrando bot...\n");
            break;
        }

        if (!pronto) {
            for (int i = 0; i < 10; i++) {
                geraNavio(tabuleiroBot, &navio[i]);  
            }
            pronto = 1;
        }

        int x, y;
        sscanf(buff, "1,%d,%d", &x, &y); // Extrai coordenadas da jogada
        printf("Jogada recebida do jogador: (%d, %d)\n", x, y);

        fazJogada(jogadaBot, NULL, 1); // Bot realiza jogada
        snprintf(buff, sizeof(buff), "Jogada do bot: (%d, %d)", jogadaBot[0][0], jogadaBot[0][1]);
        write(writefd, buff, strlen(buff)); // Envia resposta ao jogador
    }
}

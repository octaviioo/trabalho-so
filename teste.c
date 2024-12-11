#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "funcoes.h"

#define BUFFER_SIZE 256

int main() {
    int pipe1[2], pipe2[2]; // Pipes: pai -> filho (pipe1), filho -> pai (pipe2)
    pid_t pid;

    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        perror("Erro ao criar pipes");
        exit(1);
    }

    if ((pid = fork()) < 0) {
        perror("Erro ao criar subprocesso");
        exit(1);
    }

    if (pid > 0) {
        // Processo Pai (Cliente - Jogador)
        close(pipe1[0]); // Fecha leitura do pipe1
        close(pipe2[1]); // Fecha escrita do pipe2

        int tabuleiroPlayer[7][7], jogadaPlayer[7][7];
        int tabuleiroBot[7][7];
        int opcao, prontoPlayer = 0;
        char buffer[BUFFER_SIZE];

        srand(time(NULL));
        zeraMatriz(tabuleiroPlayer);
        zeraMatriz(jogadaPlayer);

        do {
            limparTela();
            printf(" Escolha uma opcao (1 ou 2, 0 para sair):\n");
            printf(" 1 >>> Jogo Modo Classico (SinglePlayer)\n");
            printf(" 2 >>> Jogo Modo Rapido (SinglePlayer)\n");

            printf(" Digite sua escolha: ");
            scanf("%d", &opcao);

            if (opcao == 0) {
                write(pipe1[1], "exit", strlen("exit") + 1); // Notifica o filho para encerrar
                break;
            }

            if (!prontoPlayer) {
                insereTabuleiroPlayer(tabuleiroPlayer);
                prontoPlayer = 1;
            }

            // Envia jogada do jogador para o filho
            fazJogada(jogadaPlayer, tabuleiroBot, 0);
            snprintf(buffer, BUFFER_SIZE, "PLAYER %d %d", jogadaPlayer[0][0], jogadaPlayer[0][1]);
            write(pipe1[1], buffer, strlen(buffer) + 1);

            // Recebe jogada do bot do filho
            read(pipe2[0], buffer, BUFFER_SIZE);
            printf("Jogada do bot: %s\n", buffer);

            // Atualiza tabuleiros e imprime
            imprimeMatriz(jogadaPlayer, 1);
        } while (opcao != 0);

        close(pipe1[1]);
        close(pipe2[0]);
    } else {
        // Processo Filho (Servidor - Bot)
        close(pipe1[1]); // Fecha escrita do pipe1
        close(pipe2[0]); // Fecha leitura do pipe2

        int tabuleiroBot[7][7], jogadaBot[7][7];
        char buffer[BUFFER_SIZE];
        Navio navio[10];
        for (int i = 0; i < 10; i++) {
            navio[i].tamanho = (i < 1) ? 4 : (i < 3) ? 3 : (i < 6) ? 2 : 1;
        }

        zeraMatriz(tabuleiroBot);
        zeraMatriz(jogadaBot);
        for (int i = 0; i < 10; i++) {
            geraNavio(tabuleiroBot, &navio[i]);
        }

        while (1) {
            read(pipe1[0], buffer, BUFFER_SIZE);

            if (strncmp(buffer, "exit", 4) == 0) {
                break; // Sai do loop ao receber comando de saída
            }

            // Processa jogada recebida
            int x, y;
            sscanf(buffer, "PLAYER %d %d", &x, &y);
            printf("Jogada do jogador: %d, %d\n", x, y);

            // Faz a jogada do bot e envia ao pai
            fazJogada(jogadaBot, tabuleiroBot, 1);
            snprintf(buffer, BUFFER_SIZE, "%d %d", jogadaBot[0][0], jogadaBot[0][1]);
            write(pipe2[1], buffer, strlen(buffer) + 1);
        }

        close(pipe1[0]);
        close(pipe2[1]);
    }

    return 0;
}

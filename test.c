#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "funcoes.h"

#define MAXBUFF 1024 // Número máximo de caracteres do buffer

// Funções
void client(int readfd, int writefd);
void server(int readfd, int writefd);
void bot(int readfd, int writefd);
int coordenadaValida(int jogadaPlayer[7][7]);
// Função principal
int main() {
    int pipe1[2], pipe2[2], pipe3[2], pipe4[2];
    int fork1, fork2;

    // Criação dos pipes
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0 || pipe(pipe3) < 0 || pipe(pipe4) < 0) {
        perror("Erro na criação dos pipes");
        exit(EXIT_FAILURE);
    }

    // Primeiro fork (pai para cliente)
    if ((fork1 = fork()) < 0) {
        perror("Erro na chamada fork1");
        exit(EXIT_FAILURE);
    }

    if (fork1 == 0) {
        // Processo Cliente
        close(pipe1[1]); // Fecha escrita em pipe1
        close(pipe2[0]); // Fecha leitura em pipe2
        close(pipe3[0]); // Fecha leitura em pipe3
        close(pipe3[1]); // Fecha escrita em pipe3
        close(pipe4[0]); // Fecha leitura em pipe4
        close(pipe4[1]); // Fecha escrita em pipe4

        client(pipe1[0], pipe2[1]);
        exit(EXIT_SUCCESS);
    }

    // Segundo fork (pai para bot)
    if ((fork2 = fork()) < 0) {
        perror("Erro na chamada fork2");
        exit(EXIT_FAILURE);
    }

    if (fork2 == 0) {
        // Processo Bot
        close(pipe1[0]); // Fecha leitura em pipe1
        close(pipe1[1]); // Fecha escrita em pipe1
        close(pipe2[0]); // Fecha leitura em pipe2
        close(pipe2[1]); // Fecha escrita em pipe2

        bot(pipe3[0], pipe4[1]);
        exit(EXIT_SUCCESS);
    }

    // Processo Pai (Servidor)
    close(pipe1[0]); // Fecha leitura em pipe1
    close(pipe2[1]); // Fecha escrita em pipe2
    close(pipe3[1]); // Fecha escrita em pipe3
    close(pipe4[0]); // Fecha leitura em pipe4

    server(pipe2[0], pipe3[1]);
    exit(EXIT_SUCCESS);
}

/* -----------------------------------------------------------------------------------------------------------
 Função Client: Executa no processo cliente.
 ----------------------------------------------------------------------------------------------------------- */
void client(int readfd, int writefd) {
    int jogadaPlayer[7][7];
    int tabuleiroBot[7][7];

    zeraMatriz(jogadaPlayer);

    printf("\nConfigurando o tabuleiro do jogador...\n");
    insereTabuleiroPlayer(jogadaPlayer);

    while (1) {
        printf("\nSua jogada:\n");
        fazJogada(jogadaPlayer, tabuleiroBot, 0);

        // Envia a jogada para o servidor
        write(writefd, jogadaPlayer, sizeof(jogadaPlayer));

        // Aguarda o bot fazer a jogada e o servidor enviar de volta o tabuleiro atualizado
        read(readfd, tabuleiroBot, sizeof(tabuleiroBot));

        printf("\nTabuleiro do Bot após sua jogada:\n");
        imprimeMatriz(tabuleiroBot, 1);
    }
}

/* -----------------------------------------------------------------------------------------------------------
 Função Server: Executa no processo servidor.
 ----------------------------------------------------------------------------------------------------------- */
void server(int readfd, int writefd) {
    int jogadaPlayer[7][7];
    int tabuleiroBot[7][7];

    zeraMatriz(tabuleiroBot);
    Navio navio[10];
    navioTamanho(navio);

    for (int i = 0; i < 10; i++) {
        geraNavio(tabuleiroBot, &navio[i]);
    }

    while (1) {
        // Recebe jogada do cliente
        read(readfd, jogadaPlayer, sizeof(jogadaPlayer));

        printf("\nServidor recebeu jogada do jogador:\n");
        imprimeMatriz(jogadaPlayer, 1);

        // Valida se a jogada é válida
        if (coordenadaValida(jogadaPlayer)) {
            // Envia a jogada para o bot
            write(writefd, jogadaPlayer, sizeof(jogadaPlayer));
        } else {
            printf("Coordenada inválida, por favor insira uma nova coordenada.\n");
            continue; // Se a coordenada for inválida, continua no loop
        }
    }
}

/* -----------------------------------------------------------------------------------------------------------
 Função Bot: Executa no processo bot.
 ----------------------------------------------------------------------------------------------------------- */
void bot(int readfd, int writefd) {
    int jogadaPlayer[7][7];
    int tabuleiroPlayer[7][7];

    zeraMatriz(tabuleiroPlayer);

    printf("\nConfigurando o tabuleiro do bot...\n");
    Navio navio[10];
    navioTamanho(navio);

    for (int i = 0; i < 10; i++) {
        geraNavio(tabuleiroPlayer, &navio[i]);
    }

    while (1) {
        // Recebe jogada do servidor
        read(readfd, jogadaPlayer, sizeof(jogadaPlayer));

        printf("\nBot recebeu jogada do jogador:\n");
        imprimeMatriz(jogadaPlayer, 1);

        // Faz sua jogada no tabuleiro do jogador
        fazJogada(jogadaPlayer, tabuleiroPlayer, 1);

        // Envia o tabuleiro atualizado para o servidor
        write(writefd, jogadaPlayer, sizeof(jogadaPlayer));
    }
}

/* -----------------------------------------------------------------------------------------------------------
 Função para validar coordenada
 ----------------------------------------------------------------------------------------------------------- */
int coordenadaValida(int jogadaPlayer[7][7]) {
    int linha, coluna;
    // Verifica se a coordenada da jogada é válida
    printf("Digite a linha e a coluna para a jogada: ");
    scanf("%d %d", &linha, &coluna);

    // Valida a coordenada (ajustar conforme a lógica do jogo)
    if (linha < 0 || linha >= 7 || coluna < 0 || coluna >= 7 || jogadaPlayer[linha][coluna] != 0) {
        return 0; // Retorna 0 para coordenada inválida
    }
    jogadaPlayer[linha][coluna] = 1; // Marca a jogada no tabuleiro
    return 1; // Retorna 1 para coordenada válida
}

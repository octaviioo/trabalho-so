#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXNUM 100

int main() {
    int pipefd1[2], pipefd2[2];  // Dois pipes: pipefd1 para pai -> filho, pipefd2 para filho -> pai
    pid_t pid;

    // Criação dos dois pipes
    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        perror("Erro ao criar pipes");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid > 0) { // Processo Pai
        close(pipefd1[0]);  // Fecha a leitura do pipe 1 (pai -> filho)
        close(pipefd2[1]);  // Fecha a escrita do pipe 2 (filho -> pai)

        char flag = 'F';        // Flag (1 byte)
        int num1 = 0, num2, num3;   // Três números inteiros
        char syncFlag = '0';    // Flag de sincronização

        while (num1 < MAXNUM) {
            // Gera 3 números
            num1 = num1 + 1;
            num2 = num1 + 1;
            num3 = num2 + 1;

            // Envia os dados para o pipe 1 (pai -> filho)
            write(pipefd1[1], &flag, sizeof(flag));       // Envia a flag
            write(pipefd1[1], &num1, sizeof(num1));       // Envia o número 1
            write(pipefd1[1], &num2, sizeof(num2));       // Envia o número 2
            write(pipefd1[1], &num3, sizeof(num3));       // Envia o número 3

            // Envia a flag de sincronização para o filho no pipe 1
            syncFlag = '1';  // Indica que o pai escreveu
            write(pipefd1[1], &syncFlag, sizeof(syncFlag));  // Sinaliza para o filho

            // Espera que o filho leia os dados e envie a confirmação de volta
            read(pipefd2[0], &syncFlag, sizeof(syncFlag));  // Espera o sinal de leitura do filho
            if (syncFlag == '0') {
                break;  // Sai do loop após o filho sinalizar
            }
        }

        close(pipefd1[1]); // Fecha a escrita do pipe 1
        close(pipefd2[0]); // Fecha a leitura do pipe 2
    } else if (pid == 0) { // Processo Filho
        close(pipefd1[1]);  // Fecha a escrita do pipe 1
        close(pipefd2[0]);  // Fecha a leitura do pipe 2

        char flag;
        int num1, num2, num3;
        char syncFlag;

        while (1) {
            // Lê os dados do pipe 1 (pai -> filho)
            read(pipefd1[0], &flag, sizeof(flag));       // Lê a flag
            read(pipefd1[0], &num1, sizeof(num1));       // Lê o número 1
            read(pipefd1[0], &num2, sizeof(num2));       // Lê o número 2
            read(pipefd1[0], &num3, sizeof(num3));       // Lê o número 3

            // Exibe os dados lidos
            printf("Filho leu:\n");
            printf("Número 1: %d\n", num1);
            printf("Número 2: %d\n", num2);
            printf("Número 3: %d\n", num3);

            // Envia a flag de sincronização para o pai no pipe 2
            syncFlag = '0';  // Indica que o filho leu os dados
            write(pipefd2[1], &syncFlag, sizeof(syncFlag)); // Sinaliza para o pai

            // Se o último número for maior ou igual a 100, o filho deve sair
            if (num1 >= MAXNUM) {
                break;  // Sai do loop quando atingir o limite
            }

            // Espera o sinal de escrita do pai para continuar o loop
            read(pipefd1[0], &syncFlag, sizeof(syncFlag));  // Espera o sinal de escrita do pai
        }

        close(pipefd1[0]); // Fecha a leitura do pipe 1
        close(pipefd2[1]); // Fecha a escrita do pipe 2
    } else {
        perror("Erro no fork");
        exit(EXIT_FAILURE);
    }

    return 0;
}

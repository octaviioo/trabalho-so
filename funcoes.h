// Define os códigos de cores ANSI
#define RESET "\033[0m"
#define CYAN "\033[34m"
#define RED "\033[31m"

typedef struct Navio {
    int x;
    int y;
    int direcao;  
    int valida;   
    int tamanho; 
} Navio;

void limparTela() {
    printf("\033[H\033[J");
}

void desenhaBarco() {
    printf(RED"                                __/__\n");
    printf(RED"                         _____/_____|           \n");
    printf(RED"                 _______/_____\\______\\_____\n");
    printf(RED"                 \\                 < < <    |\n");
    printf(CYAN"               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf(RESET);
    printf(RED"  _           _        _ _                                    _ \n");
    printf(RED" | |         | |      | | |                                  | |\n");
    printf(RED" | |__   __ _| |_ __ _| | |__   __ _   _ __   __ ___   ____ _| |\n");
    printf(RED" | '_ \\ / _` | __/ _` | | '_ \\ / _` | | '_ \\ / _` \\ \\ / / _` | |\n");
    printf(RED" | |_) | (_| | || (_| | | | | | (_| | | | | | (_| |\\ V / (_| | |\n");
    printf(RED" |_.__/ \\__,_|\\__\\__,_|_|_| |_|\\__,_| |_| |_|\\__,_| \\_/ \\__,_|_|\n");
    printf(RESET);
}

void tabelaTamanhos(){
    printf("Monte seu Tabuleiro de navios:\n\n");
    printf("Tabela de navios:\n");
    printf("Quantidade || Tamanho\n");
    printf("   1       ||    4\n");
    printf("   2       ||    3\n");
    printf("   3       ||    2\n");
    printf("   4       ||    1\n");
}


void draw_box_with_text(int width, int height, const char *text) {
    int text_row = height / 2;               
    int text_col = (width - strlen(text)) / 2; 
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i == 0 || i == height - 1) {
                printf(CYAN "*"); 
            } else if (j == 0 || j == width - 1) {
                printf(CYAN "*"); 
            } else if (i == text_row && j >= text_col && j < text_col + strlen(text)) {
                printf(RESET "%c", text[j - text_col]); 
            } else {
                printf(" ");
            }
        }
        printf(RESET "\n"); 
    }
}

void zeraMatriz(int matriz[7][7]) {
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            matriz[i][j] = 0;
        }
    }
}

void imprimeMatriz(int matriz[7][7], int tipo){

    if(!tipo){
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if(matriz[i][j])
                printf(RED"%d ", matriz[i][j]);
                else printf(CYAN"%d ", matriz[i][j]);
            }
            printf(RESET"\n");
        }
    }else{
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                switch (matriz[i][j])
                {
                case 1:
                    printf(CYAN"%d ", matriz[i][j]);
                    break;

                case 2:
                    printf(RED"%d ", matriz[i][j]);
                    break;
                
                default:
                    printf(RESET"%d ", matriz[i][j]);
                    break;
                }      
            }
            printf("\n");
        }
    }

    
}

int verificaPosicaoValida(int tabuleiro[7][7], Navio navio) {
    if (navio.direcao == 0) {  
        if (navio.y + navio.tamanho > 7) {  
            return 0;
        }
        for (int i = 0; i < navio.tamanho; i++) {
            if (tabuleiro[navio.x][navio.y + i] != 0) { 
                return 0;
            }
        }
    } else {
        if (navio.x + navio.tamanho > 7) {  
            return 0;
        }
        for (int i = 0; i < navio.tamanho; i++) {
            if (tabuleiro[navio.x + i][navio.y] != 0) {  
                return 0;
            }
        }
    }
    return 1;  
}

void geraNavio(int tabuleiro[7][7], Navio *navio) {

    navio->valida = 0;

    while (!navio->valida) {
        navio->x = rand() % 7; 
        navio->y = rand() % 7;   
        navio->direcao = rand() % 2; 
        navio->valida = 0;  

        if (verificaPosicaoValida(tabuleiro, *navio)) {
            navio->valida = 1;
           
            for (int i = 0; i < navio->tamanho; i++) {
                if (navio->direcao == 0) { 
                    tabuleiro[navio->x][navio->y + i] = 1;
                } else {  
                    tabuleiro[navio->x + i][navio->y] = 1;
                }
            }
        }
    }
}

void insereNavioPlayer(int tabuleiro[7][7], Navio navio){

    for (int i = 0; i < navio.tamanho; i++) {
        if (navio.direcao == 0) { 
                tabuleiro[navio.x][navio.y + i] = 1;
        } else {  
            tabuleiro[navio.x + i][navio.y] = 1;
       }
    }
}

void insereTabuleiroPlayer(int tabuleiroPlayer[7][7]){

    Navio navioPlayer[10];
    int validaPlayer = 0;

    for (int i = 0; i < 10; i++)
    {
        if (i == 0)
        {
            navioPlayer[i].tamanho = 4;
        }
        else if (i == 1 || i == 2)
        {
            navioPlayer[i].tamanho = 3;
        }
        else if (i >= 3 && i <= 5)
        {
            navioPlayer[i].tamanho = 2;
        }
        else
        {
            navioPlayer[i].tamanho = 1;
        }

        do
        {

            limparTela();
            tabelaTamanhos();
            printf("\nSeu tabuleiro atual:\n");
            imprimeMatriz(tabuleiroPlayer, 0);

            printf("\nInsira o navio %d\n", i);

            printf("Digite o valor da posição x: ");
            scanf("%d", &navioPlayer[i].x);

            printf("Digite o valor da posição y: ");
            scanf("%d", &navioPlayer[i].y);

            do
            {
                printf("Digite a orientacao do navio\n0 para vertical e 1 para horizontal: ");
                scanf("%d", &navioPlayer[i].direcao);
                if (navioPlayer[i].direcao < 0 || navioPlayer[i].direcao > 1)
                {
                    printf("\nPosicao Invalida. Digite novamente...\n");
                }
            } while (navioPlayer[i].direcao < 0 || navioPlayer[i].direcao > 1);

            validaPlayer = verificaPosicaoValida(tabuleiroPlayer, navioPlayer[i]);

            if (!validaPlayer)
            {
                printf("\nposicao invalida, digite novamente...\n");
            }
            else
            {
                insereNavioPlayer(tabuleiroPlayer, navioPlayer[i]);
            }

        } while (!validaPlayer);
    }
}

void fazJogada(int jogada[7][7], int tabuleiro[7][7], int tipo)
{

    int cooJogada[2], validaJogada = 1, validaJogadaPlayer = 0;


    //tipo 0 humano
    //tipo 1 bot
    /*
    Valor 0 -> inexplorado
    Valor 1 -> Agua
    Valor 2 -> Barco
    */

    while (validaJogada)
    {
        if(tipo){
            cooJogada[0] = rand() % 7;
            cooJogada[1] = rand() % 7;
        }else{

            do{
                printf("Faca sua jogada\n");
                printf("Valor da linha: ");
                scanf("%d", &cooJogada[0]);
                printf("Valor da coluna: ");
                scanf("%d", &cooJogada[1]);
                if(cooJogada[0] > 7 || cooJogada[0] < 0 || cooJogada[1] > 7|| cooJogada[1] < 0){
                    validaJogadaPlayer = 1;
                    printf("\n Valores invalidos, digite numeros entre 0 e 6\n");
                }
            }while (validaJogadaPlayer);
        }
        if (jogada[cooJogada[0]][cooJogada[1]] == 0)
        {
            if (tabuleiro[cooJogada[0]][cooJogada[1]])
            {
                jogada[cooJogada[0]][cooJogada[1]] = 2;
            }
            else
            {
                jogada[cooJogada[0]][cooJogada[1]] = 1;
            }
            validaJogada = 0;
        }
        if(!tipo && validaJogada){
            printf("Coordenada ja explorada, insira outra\n\n");
        }
    }
}

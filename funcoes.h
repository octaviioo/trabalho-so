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
                printf("%d ", matriz[i][j]);
            }
            printf("\n");
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

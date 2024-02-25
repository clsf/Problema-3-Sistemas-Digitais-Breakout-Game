#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <intelfpgaup/KEY.h>
#include <intelfpgaup/video.h>
#include <intelfpgaup/accel.h>

/*
Lógica dos blocos
*/

#define WIDTH 320
#define BLOCKWIDTH 34
#define BLOCKHEIGHT 10 


int quantidadeBlocosHorizontal = (WIDTH - 14)/BLOCKWIDTH;  //quantidade de blocos em cada linha
int quantidadeBlocosVertical = 7; //quantidade de blocos Verticalmente ou em cada coluna
int quantidadeBlocosGlobal;


// Definindo a estrutura mensagem
struct Message {
    int x;
    int y;
    char mensagem[50]; // Ajuste o tamanho conforme necessário
    int ativa;
  void (*exibirMensagem)(struct Message *);
};


struct Pontuacao {
    int x;
    int y;
    int pontos;

    // Método para atualizar os pontos
    void (*atualizarPontos)(struct Pontuacao *);

    // Método para exibir a pontuação na tela
    void (*exibirPontuacao)(struct Pontuacao *);
};


/*
Lógica da Barra
*/
typedef struct
{
    int width;
    int x1;
    int y1;
    int x2;
    int y2;
    short color;
} Paddle;

typedef struct
{
    int limitLeft;
    int limitRight;
} WallPaddle;

typedef struct {
    int pos_x1;
    int pos_y1;
    int pos_x2;
    int pos_y2;
    int acel_x;
    int acel_y;
    short color;
} Ball;

typedef struct {
    int x1;
    int y1;
    int x2;
    int y2;
    short color;
} Wall;

//Estrutura do Bloco
typedef struct 
{
    int x1Bloco;  
    int x2Bloco;
    int y1Bloco;
    int y2Bloco; 
    int ativo;
    short cor;   

} Bloco;

// Método para exibir ou ocultar a mensagem na tela
void exibirMensagem(struct Message* msg) {
    if (msg->ativa) {
        video_text(msg->x, msg->y, msg->mensagem);
        video_show ( );
      }
}

void atualizarPontos(struct Pontuacao *pontuacao) {
    pontuacao->pontos++;
}


void exibirPontuacao(struct Pontuacao *pontuacao) {
    char mensagem[50];
    sprintf(mensagem, "Score: %d", pontuacao->pontos);
    video_text(pontuacao->x, pontuacao->y, mensagem);
}

//Função responsável por criar um bloco a partir de parâmetros recebidos
Bloco setBloco(int x1, int x2, int y1, int y2, short color){ 

    Bloco B; 
    B.x1Bloco = x1;
    B.x2Bloco = x2; 
    B.y1Bloco = y1; 
    B.y2Bloco = y2;  
    B.cor =  color;
    B.ativo = 1;

    return B;
};

//Função auxiliar que verifica se o bloco está ativo para colocar as informações dentro do buffer para ser mostrado na tela
void desenharBlocoVGA(Bloco b){ 
	
   if( b.ativo ){
    video_box(b.x1Bloco, b.y1Bloco, b.x2Bloco, b.y2Bloco, b.cor);
   };
};

//Função responsável colocar direcionar todos os blocos de um array para uma função coloca as informações do bloco em um buffer para ser mostrado na tela 
void desenharBlocos(Bloco* Blocos){
    int i = 0;
    for (i; i < quantidadeBlocosGlobal; i++){
        
        desenharBlocoVGA(Blocos[i]);

    }
};

//Função responspavel por criar o array de blocos
void CriarBlocos(Bloco* blocos){

    int coordXInicial = 0 + 7; // coordenada x que inicia a linha dos blocos
    int coordXFinal =  WIDTH - 7; // coordenada x que termina a linha dos blocos
    int coordYInicial = 0 + 6; //coordenada y onde se inicia a coluna de blocos
    int coordYFinal =  coordYInicial + (quantidadeBlocosVertical * BLOCKHEIGHT); //coordenada y onde termina a coluna de blocos

    Bloco B1;
    int indexBlocos = 0;
    int i = coordYInicial;
    int j = coordXInicial;
    int linhas = 0; 
    short cores[7] = {video_PINK, video_RED, video_ORANGE, video_YELLOW, video_GREEN, video_CYAN, video_BLUE}; //Array de cores para os blocos
    int l, m;

    for( i ; i < coordYFinal + (quantidadeBlocosVertical); i += BLOCKHEIGHT + 1){ //For das posições verticais dos blocos
        for (j = coordXInicial; j < coordXFinal; j += BLOCKWIDTH){ // For das posições horizontais dos blocos
            
            l = j + BLOCKWIDTH; //x2 final do bloco
            m = i + BLOCKHEIGHT; //y2 final do bloco

            //Criando Bloco
            B1 = setBloco(j, l, i, m, cores[linhas]);   
            //Adicionando bloco a lista de Blocos    
            blocos[indexBlocos] = B1; 
            indexBlocos += 1;

        }
        linhas +=1;
                
    }
};

int colisao_blocos(Ball* ball, Bloco *blocos) {
    int i = 0;
    for (i;i<quantidadeBlocosGlobal;i++) {
        if (colide_bloco(ball, &blocos[i]) == 1) {
            return 1;
        }
    }
    return 0;
}

int colide_bloco(Ball* ball, Bloco* bloco) {
    int ball_x1 = ball->pos_x1 + ball->acel_x;
    int ball_y1 = ball->pos_y1 + ball->acel_y;
    int ball_x2 = ball->pos_x2 + ball->acel_x;
    int ball_y2 = ball->pos_y2 + ball->acel_y;
    if (bloco->ativo == 1 && ((ball_x1 >= bloco->x1Bloco && ball_x2 <= bloco->x2Bloco)||(ball->pos_x1 >= bloco->x1Bloco && ball->pos_x2 <= bloco->x2Bloco)) && ball_y1 >= bloco->y1Bloco && ball_y2 <= bloco->y2Bloco) {
        bloco->ativo = 0;
        if (ball->pos_x1 >= bloco->x1Bloco && ball->pos_x2 <= bloco->x2Bloco) {
            ball->acel_y = -ball->acel_y;
        } else if (ball->pos_y1 >= bloco->y1Bloco && ball->pos_y2 <= bloco->y2Bloco) {
            ball->acel_x = -ball->acel_x;
        } else {
            ball->acel_y = -ball->acel_y;
            ball->acel_x = -ball->acel_x;
        };
        return 1;
    };
    return 0;
}

//Função responsável por mudar o status de todos os blocos para ativo para que sejam mostrados no monitor
void ativar_blocos(Bloco *blocos) {
    int i = 0;
    for (i; i < quantidadeBlocosGlobal; i++){
        blocos[i].ativo = 1;
    }
}

/*
Lógica da Bola
*/

void ball_move(Ball *ball);
int colide(Ball *ball, Wall *wall);

void ball_move(Ball *ball) {
    ball->pos_x1 += ball->acel_x;
    ball->pos_y1 += ball->acel_y;
    ball->pos_x2 += ball->acel_x;
    ball->pos_y2 += ball->acel_y;
}

int colide(Ball *ball, Wall *wall) {
    int ball_x1 = ball->pos_x1 + ball->acel_x;
    int ball_y1 = ball->pos_y1 + ball->acel_y;
    int ball_x2 = ball->pos_x2 + ball->acel_x;
    int ball_y2 = ball->pos_y2 + ball->acel_y;

    

    if (ball_x1 >= wall->x1 && ball_x2 <= wall->x2 && ball_y1 >= wall->y1 && ball_y2 <= wall->y2) {
        if (ball->pos_x1 >= wall->x1 && ball->pos_x2 <= wall->x2) {
            ball->acel_y = -ball->acel_y;
        } else if (ball_y1 >= 215 && ball_y2 <= 220) {
            ball->acel_y = -ball->acel_y;
            ball->acel_x = -ball->acel_x;
        } else {
            ball->acel_x = -ball->acel_x;
        };
        return 1;
    };
    return 0;
}

void drawPaddle(Paddle *player){
    video_box(player->x1, player->y1, player->x2, player->y2, player->color); // Desenha uma caixa preenchida
}


void movePaddle(Paddle *player, WallPaddle *wall){

    int accel_x, accel_y, accel_z, accel_ready, accel_tap, accel_dtap, accel_msg;
    
    
    accel_read(&accel_ready, &accel_tap, &accel_dtap, &accel_x, &accel_y, &accel_z, &accel_msg);
    accel_x = accel_x/12;
    if(player->x1 + accel_x  <= wall->limitLeft || player->x2 + accel_x >= wall->limitRight){
        player->x1 += 0;
        player->x2 += 0;
    }
    else{
        player->x1 += accel_x;
        player->x2 += accel_x;
    }
	video_box((*player).x1, (*player).y1, (*player).x2, (*player).y2, (*player).color); // Desenha uma caixa preenchida
}

void colidePaddle(Ball *ball, Paddle *paddle) {
    int ball_x1 = ball->pos_x1 + ball->acel_x;
    int ball_y1 = ball->pos_y1 + ball->acel_y;
    int ball_x2 = ball->pos_x2 + ball->acel_x;
    int ball_y2 = ball->pos_y2 + ball->acel_y;
    
    if (((ball_x1 >= paddle->x1 && ball_x2 <= paddle->x2)||(ball->pos_x1 >= paddle->x1 && ball->pos_x2 <= paddle->x2)) && ball_y1 >= paddle->y1 && ball_y2 <= paddle->y2) {
        if (ball->pos_x1 >= paddle->x1 && ball->pos_x2 <= paddle->x2) {
            ball->acel_y = -ball->acel_y;
            if(ball_x1 >= paddle->x1 &&  ball_x2 <= (paddle->x1 + (paddle->width/2))){
                ball->acel_x = (-1) * abs(ball->acel_x);
            }
            else{
                ball->acel_x = abs(ball->acel_x);
            }
        } else {
            ball->acel_x = -ball->acel_x;
        };
    };
}

int main() {
    int game_loop = 1;
    int screen_x, screen_y;
    int char_x, char_y;
    int key_data;
    int pause = 0;
    int game_over = 0;


    // Inicializando o Botões
    KEY_open();

    // Inicializando o VGA
    video_open();
    video_read (&screen_x, &screen_y, &char_x, &char_y);
    video_erase();
    video_clear ( ); // clear current VGA Back buffer
    video_show ( ); // swap Front/Back to display the cleared buffer
    video_clear ( ); // clear the VGA Back buffer, where we will draw lines

    // Inicializando o Acelerômetro
    accel_open();
    accel_init ();
    accel_calibrate ();
    accel_format (1, 8);

    Ball ball;
    ball.pos_x1 = (screen_x-1)/2;
    ball.pos_y1 = ((screen_y-2)/2);
    ball.pos_x2 = (screen_x-1)/2+2;
    ball.pos_y2 = ((screen_y-2)/2)+2;
    ball.acel_x = +1;
    ball.acel_y = +1;
    ball.color = video_RED;

    Wall r_wall;
    r_wall.x1 = 314;
    r_wall.y1 = 0;
    r_wall.x2 = screen_x-1;
    r_wall.y2 = screen_y-1;
    r_wall.color = video_GREY;

    Wall t_wall;
    t_wall.x1 = 0;
    t_wall.y1 = 0;
    t_wall.x2 = screen_x-1;
    t_wall.y2 = 5;
    t_wall.color = video_GREY;
    

    Wall l_wall;
    l_wall.x1 = 0;
    l_wall.y1 = 0;
    l_wall.x2 = 6;
    l_wall.y2 = screen_y-1;
    l_wall.color = video_GREY;

    Wall b_wall;
    b_wall.x1 = 0;
    b_wall.y1 = screen_y - 5 - 1;
    b_wall.x2 = screen_x-1;
    b_wall.y2 = screen_y-1;
    b_wall.color = 0;

    WallPaddle wallPad = {6, 314};
    Paddle player = {50, ((screen_x -2)/2) - (player.width) + 10, 215, (player.x1 + player.width), 220, video_RED};

    struct Message game_over_msg = {char_x/2 - 20, char_y/2, "Game Over! Aperte Key 3 para reiniciar", 0};
    struct Message win_msg = {char_x/2 - 25, char_y/2, "Parabens Voce Ganhou! Aperte Key 3 para reiniciar", 0};
    struct Pontuacao pontuacao = {2, char_y - 3, 0};

    /*
    Blocos
    */

    quantidadeBlocosGlobal = quantidadeBlocosHorizontal * quantidadeBlocosVertical;
    Bloco blocos[quantidadeBlocosGlobal];
    CriarBlocos(blocos);


    video_show(); // Troca Front/Back para exibir a mensagem
    
    KEY_read(&key_data);
    while(key_data != 8){
        video_text(char_x/2 - 10, char_y/2, "Aperte Key 3 para iniciar");
        video_show();
        KEY_read(&key_data);
        //printf("Botão: %i\n", key_data);
    }
    video_erase();

    while(game_loop == 1){


        video_clear();

        video_box(ball.pos_x1, ball.pos_y1, ball.pos_x2, ball.pos_y2, ball.color);

        video_box(r_wall.x1, r_wall.y1, r_wall.x2, r_wall.y2, r_wall.color);
        video_box(t_wall.x1, t_wall.y1, t_wall.x2, t_wall.y2, t_wall.color);
        video_box(l_wall.x1, l_wall.y1, l_wall.x2, l_wall.y2, l_wall.color);
        video_box(b_wall.x1, b_wall.y1, b_wall.x2, b_wall.y2, b_wall.color);

        desenharBlocos(blocos);

        KEY_read(&key_data);
        if(key_data == 8){
            pause = 1;
            while(pause == 1){
                KEY_read(&key_data);
                //printf("Botão: %i\n", key_data);
                if(key_data == 8){
                    pause = 0;
                }
            }
        }
        
        colide(&ball, &r_wall);
        colide(&ball, &t_wall);
        colide(&ball, &l_wall);
        colidePaddle(&ball, &player);
        game_over = colide(&ball, &b_wall);
        if (game_over == 1) {
            pause = 1;
            while(pause == 1){
                game_over_msg.ativa = 1;
                video_clear();
                exibirMensagem(&game_over_msg);
                KEY_read(&key_data);
                //printf("Botão: %i\n", key_data);
                if(key_data == 8){
                    pause = 0;
                }
            }
            game_over_msg.ativa = 0;
            ativar_blocos(blocos);
            ball.pos_x1 = (screen_x-1)/2;
            ball.pos_y1 = ((screen_y-2)/2);
            ball.pos_x2 = (screen_x-1)/2+2;
            ball.pos_y2 = ((screen_y-2)/2)+2;
            ball.acel_x = +1;
            ball.acel_y = +1;

            player.x1 =  ((screen_x -2)/2) - (player.width);
            player.x2 =  ((player.x1 + player.width));

            pontuacao.pontos = 0;

            video_erase();
        }

        if (colisao_blocos(&ball, blocos) == 1) {
            atualizarPontos(&pontuacao);
        }
        exibirPontuacao(&pontuacao);

        if (pontuacao.pontos == quantidadeBlocosGlobal) {
            pause = 1;
            while(pause == 1){
                win_msg.ativa = 1;
                video_clear();
                exibirMensagem(&win_msg);
                KEY_read(&key_data);
                //printf("Botão: %i\n", key_data);
                if(key_data == 8){
                    pause = 0;
                }
            }
            win_msg.ativa = 0;
            ativar_blocos(blocos);
            ball.pos_x1 = (screen_x-1)/2;
            ball.pos_y1 = (screen_y-1-25);
            ball.pos_x2 = (screen_x-1)/2+2;
            ball.pos_y2 = (screen_y-1-25)+2;
            ball.acel_x = +1;
            ball.acel_y = +1;

            player.x1 =  ((screen_x -2)/2) - (player.width);
            player.x2 =  ((player.x1 + player.width));

            pontuacao.pontos = 0;

            video_erase();
        }

        movePaddle(&player, &wallPad);
        ball_move(&ball);

        video_show();
    }

    video_close();
    accel_close();
    KEY_close();
    return 0;
}
#include <allegro5/allegro5.h>				//Biblioteca base do Allegro
#include <allegro5/allegro_font.h>			//Biblioteca de fontes do Allegro
#include <allegro5/allegro_primitives.h>	//Biblioteca de figuras básicas
#include <allegro5/allegro_image.h>

#include <allegro5/allegro_native_dialog.h>

#include <stdio.h>
#include <stdlib.h>

#include "Player.h"							//Inclusão da biblioteca de quadrados
#include "Enemy.h"
#include "Bullet.h"
#include "Obstacle.h"


#define X_SCREEN 1000		                //Definição do tamanho da tela em pixels no eixo x
#define Y_SCREEN 800		                //Definição do tamanho da tela em pixels no eixo y

//Implementação da função que atualiza o posicionamento de projéteis conforme o movimento dos mesmos (!)
void update_bullets(player *player, enemy *enemy){									
	player->gun->shots = bullet_move(player->gun->shots, Y_SCREEN);
	enemy->gun->shots = bullet_move(enemy->gun->shots, Y_SCREEN);
}

//Implementação da função que verifica se um projétil acertou um inimigo (!)
void check_enemy_kill(player *player, enemy_node *enemies){
    bullet *previous, *index;
    for(enemy_node *aux = enemies; aux != NULL; aux = (enemy_node*)aux->next){
        previous = NULL;
        index = player->gun->shots;
        while(index != NULL){		    //Para todos os projéteis do atirador (!)
		    if ((index->x >= aux->x - aux->side_x/2) && (index->x <= aux->x + aux->side_x/2) &&     //Verique se houve colisão com a vítima no eixo X (!)
		    (index->y >= aux->y - aux->side_y/2) && (index->y <= aux->y + aux->side_y/2)){	    //Verifique se houve colisão com a vítima no eixo Y (!)
                aux->life = 0;																			        //Se sim, retorne que o quadrado vítima foi atingido
		    	if (previous){											//Verifica se não é o primeiro elemento da lista de projéteis (!)
				    previous->next = index->next;						//Se não for, salva o próximo projétil (!)
				    bullet_destroy(index);								//Chama o destrutor para o projétil atual (!)
				    index = (bullet*) previous->next;					//Atualiza para o próximo projétil (!)
			    }
			    else {													//Se for o primeiro projétil da lista (!)
				    player->gun->shots = (bullet*) index->next;			//Atualiza o projétil no início da lista (!)
				    bullet_destroy(index);								//Chama o destrutor para o projétil atual (!)
				    index = player->gun->shots;							//Atualiza para o próximo projétil (!)
			    }
            }
            else{
                previous = index;
                index = (bullet*) index->next;
            }
        }
    }																			            //Se não houver nenhum projétil que acertou a vítima, retorne falso (!)
}

void check_player_kill(player *player, enemy *enemies){	//Implementação da função que verifica se um projétil acertou um jogador (!)
    bullet *previous = NULL, *index;
        index = enemies->gun->shots;
        while(index != NULL){		    //Para todos os projéteis do atirador (!)
		    if ((index->x >= player->x - player->side_x/2) && (index->x <= player->x + player->side_x/2) &&     //Verique se houve colisão com a vítima no eixo X (!)
		   (index->y >= player->y - player->side_y/2) && (index->y <= player->y + player->side_y/2)){	    //Verifique se houve colisão com a vítima no eixo Y (!)

                player->life= player->life - 1;																			        //Se sim, retorne que o quadrado vítima foi atingido
		    	if (previous){											//Verifica se não é o primeiro elemento da lista de projéteis (!)
				    previous->next = index->next;						//Se não for, salva o próximo projétil (!)
				    bullet_destroy(index);								//Chama o destrutor para o projétil atual (!)
				    index = (bullet*) previous->next;					//Atualiza para o próximo projétil (!)
			    }
			    else {													//Se for o primeiro projétil da lista (!)
				    enemies->gun->shots = (bullet*) index->next;			//Atualiza o projétil no início da lista (!)
				    bullet_destroy(index);								//Chama o destrutor para o projétil atual (!)
				    index = enemies->gun->shots;							//Atualiza para o próximo projétil (!)
			    }
            }
            else{
                previous = index;
                index = (bullet*) index->next;
            }
        }
																        //Se não houver nenhum projétil que acertou a vítima, retorne falso (!)
}

unsigned char shot_colun_player(player* player){
    for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next){		        //Para todos os projéteis do atirador (!)
		if ((index->x >= player->x - 48) && (index->x <= player->x + 48))   //Verique se ha disparos do player na coluna do eixo X (!)
                return 1;																			    //Se sim, retorna 1
		}
	return 0;		

}
unsigned char shot_colun_enemy(enemy* element, enemy_node *nodo){
    unsigned char dool = 0;
    for (bullet *index = element->gun->shots; index != NULL; index = (bullet*) index->next){		        //Para todos os projéteis do atirador (!)
		if ((index->x >= nodo->x - 48) && (index->x <= nodo->x + 48)){                                       //Verique se ha disparos na coluna do eixo X (!)
            if((nodo->type >1) || (dool>0))    
                return 1;																			        //Se sim, retorna 1
            else
                dool++;
        }
    }
    if(nodo->type > 2){
        for (enemy_node *index = element->ini; index != NULL; index = (enemy_node*) index->next){		        //Para todos os projéteis do atirador (!)
	    	if ((index->x >= nodo->x - 48) && (index->x <= nodo->x + 48) &&
                    !((index->y >= nodo->y - 32)) && (!(index->y <= nodo->y + 32)))                                       //Verique se ha disparos na coluna do eixo X (!)
                    return 1;																			        //Se sim, retorna 1
        }
    }

	return 0;		

}

void check_obstacle(pistol *gun, obstacle *obex){
    bullet *previous, *index;
    for(obstacle *aux = obex; aux != NULL; aux = (obstacle*)aux->next){
        previous = NULL;
        index = gun->shots;
        while(index != NULL){		    //Para todos os projéteis do atirador (!)
		    if ((index->x >= aux->x - aux->side/2) && (index->x <= aux->x + aux->side/2) &&     //Verique se houve colisão com a vítima no eixo X (!)
		    (index->y >= aux->y - aux->side/2) && (index->y <= aux->y + aux->side/2)){	    //Verifique se houve colisão com a vítima no eixo Y (!)
                if(index->type >=3)
                    aux->life = aux->life-2;																			        //Se sim, retorne que o quadrado vítima foi atingido
                else
                    aux->life--;
                
                if (previous){											//Verifica se não é o primeiro elemento da lista de projéteis (!)
				    previous->next = index->next;						//Se não for, salva o próximo projétil (!)
				    bullet_destroy(index);								//Chama o destrutor para o projétil atual (!)
				    index = (bullet*) previous->next;					//Atualiza para o próximo projétil (!)
			    }
			    else {													//Se for o primeiro projétil da lista (!)
				    gun->shots = (bullet*) index->next;			//Atualiza o projétil no início da lista (!)
				    bullet_destroy(index);								//Chama o destrutor para o projétil atual (!)
				    index = gun->shots;							//Atualiza para o próximo projétil (!)
			    }
            }
            else{
                previous = index;
                index = (bullet*) index->next;
            }
        }
    }
}


unsigned char update_position(player *player, enemy* enemy, obstacle *obex){	//Função de atualização das posições dos quadrados conforme os comandos do controle
    unsigned char kill = 0, disp = 0;

	if (player->control->left){									//Se o botão de movimentação para esquerda do controle do primeiro jogador está ativado...
		player_move(player, 1, 0, X_SCREEN, Y_SCREEN);			//Move o quadrado do primeiro jogador para a esquerda
	}
	if (player->control->right){								//Se o botão de movimentação para direita do controle do primeir ojogador está ativado...
		player_move(player, 1, 1, X_SCREEN, Y_SCREEN);			//Move o quadrado do primeiro jogador para a direta
	}

	if (player->control->fire){									//Verifica se o primeiro jogador está atirando (!)
		if (!shot_colun_player(player)){								//Verifica se a arma do primeiro jogador não está em cooldown (!)
			player_shot(player); 								//Se não estiver, faz um disparo (!)

		} 
	}
    if (enemy->gun->timer==0){
        for(enemy_node *index = enemy->ini; index!=NULL;index = (enemy_node*)index->next){
		    if ((!shot_colun_enemy(enemy, index)) && (disp < 2)){								//Verifica se a arma do primeiro jogador não está em cooldown (!)
	    		enemy_shot(enemy, index); 								    //Se não estiver, faz um disparo (!)
                disp++;
                enemy->gun->timer = 30;
            } 
        }
    }

    update_bullets(player,enemy);								//Atualiza os disparos do primeiro jogador (!)
    check_obstacle(player->gun, obex);
    check_obstacle(enemy->gun, obex);
    check_enemy_kill(player, enemy->ini);
    check_player_kill(player, enemy);
    enemy->gun->timer--;
    if((enemy_move(enemy, 1, X_SCREEN, Y_SCREEN)))
        return 1;


    return kill;
}

enemy_node *dst_enemy_node(enemy_node* ant, enemy_node* nodo){
    enemy_node *aux = nodo;
    nodo = (enemy_node*)nodo->next;
    if(ant != NULL){
        ant->next = (struct enemy_node*)nodo;
    }
    free(aux);
    return nodo;
}



size_t remove_kill_enemies(enemy *element,size_t score){
    enemy_node *ant = NULL, *nodo = element->ini;
    while(nodo!=NULL){
        if(nodo->life <= 0){
            if(nodo->type>=3)
                score = score + 10;
            else 
                score = score + 30;
            nodo = dst_enemy_node(ant, nodo);
            if(ant == NULL)
                element->ini = nodo;
        }
        else{
            ant = nodo;
            nodo = (enemy_node*)nodo->next;
        }
    }
    return score;
}

obstacle *dst_obstacle_node(obstacle* ant, obstacle* nodo){
    obstacle *aux = nodo;
    nodo = (obstacle*)nodo->next;
    if(ant != NULL){
        ant->next = (struct obstacle*)nodo;
    }
    free(aux);
    return nodo;
}

obstacle *remove_obstacles(obstacle *element){
    obstacle *ant = NULL, *nodo = element;
    while(nodo!=NULL){
        if(nodo->life <= 0){
            nodo = dst_obstacle_node(ant, nodo);
            if(ant == NULL)
                element = nodo;
        }
        else{
            ant = nodo;
            nodo = (obstacle*)nodo->next;
        }
    }
    return element;
}





int main(){
    size_t score = 0;
    char sc[1024];
    ALLEGRO_BITMAP *playerBit = NULL;
    ALLEGRO_BITMAP *lifeBit = NULL;

    ALLEGRO_BITMAP *bar1 = NULL;
    ALLEGRO_BITMAP *bar2 = NULL;
    ALLEGRO_BITMAP *bar3 = NULL;
    ALLEGRO_BITMAP *bar4 = NULL;
    ALLEGRO_BITMAP *bar5 = NULL;



    ALLEGRO_BITMAP *enemyBit1 = NULL;
    ALLEGRO_BITMAP *enemyBit1_2 = NULL;
    ALLEGRO_BITMAP *enemyBit2 = NULL;
    ALLEGRO_BITMAP *enemyBit2_2 = NULL;
    ALLEGRO_BITMAP *enemyBit3 = NULL;
    ALLEGRO_BITMAP *enemyBit3_2 = NULL;


	al_init();																								//Faz a preparação de requisitos da biblioteca Allegro
	al_init_primitives_addon();																				//Faz a inicialização dos addons das imagens básicas
	al_init_image_addon();                                                                                  //Faz a inicializacao das bitmaps
	al_install_keyboard();																					//Habilita a entrada via teclado (eventos de teclado), no programa

    playerBit = al_load_bitmap("player.bmp");
    lifeBit = al_load_bitmap("life.bmp");

    bar1 = al_load_bitmap("bar1.bmp");
    bar2 = al_load_bitmap("bar2.bmp");
    bar3 = al_load_bitmap("bar3.bmp");
    bar4 = al_load_bitmap("bar4.bmp");
    bar5 = al_load_bitmap("bar5.bmp");

    enemyBit1 = al_load_bitmap("enemy1.bmp");
    enemyBit1_2 = al_load_bitmap("enemy1_2.bmp");
    enemyBit2 = al_load_bitmap("enemy2.bmp");
    enemyBit2_2 = al_load_bitmap("enemy2_2.bmp");
    enemyBit3 = al_load_bitmap("enemy3.bmp");
    enemyBit3_2 = al_load_bitmap("enemy3_2.bmp");





	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 30.0);														//Cria o relógio do jogo; isso indica quantas atualizações serão realizadas por segundo (30, neste caso)
	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();													//Cria a fila de eventos; todos os eventos (programação orientada a eventos) 
	ALLEGRO_FONT* font = al_create_builtin_font();															//Carrega uma fonte padrão para escrever na tela (é bitmap, mas também suporta adicionar fontes ttf)
	ALLEGRO_DISPLAY* disp = al_create_display(X_SCREEN, Y_SCREEN);										    //Cria uma janela para o programa, define a largura (x) e a altura (y) da tela em píxeis (320x320, neste caso)

	al_register_event_source(queue, al_get_keyboard_event_source());									    //Indica que eventos de teclado serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_display_event_source(disp));										//Indica que eventos de tela serão inseridos na nossa fila de eventos
	al_register_event_source(queue, al_get_timer_event_source(timer));								        //Indica que eventos de relógio serão inseridos na nossa fila de eventos

	player* player = player_create(32, 32, 0, X_SCREEN/2, Y_SCREEN-16, X_SCREEN, Y_SCREEN);					//Cria a entidade do jogador
	if (!player) 
        return 1;																				    //Verificação de erro na criação do primeiro jogador

    enemy* enemies = enemy_create(X_SCREEN, Y_SCREEN);
    obstacle* obex = create_obex(X_SCREEN, Y_SCREEN, 32);


	ALLEGRO_EVENT event;																					//Variável que guarda um evento capturado, sua estrutura é definida em: https:		//www.allegro.cc/manual/5/ALLEGRO_EVENT
	al_start_timer(timer);																				    //Função que inicializa o relógio do programa
	unsigned char kill, frame = 0;															//Variáveis de controle de vida do jogador
    while(1){

   while(1){																							    //Laço servidor do jogo
		al_wait_for_event(queue, &event);																    //Função que captura eventos da fila, inserindo os mesmos na variável de eventos

        score = remove_kill_enemies(enemies,score);
        obex = remove_obstacles(obex);
		if (player->life<=0){																					//Verifica se o jogador foi morto 
			al_clear_to_color(al_map_rgb(0, 0, 0));															//Limpe a tela atual para um fundo preto
            al_draw_text(font, al_map_rgb(0, 0, 255), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "JOGADOR PERDEU!");	        //Se o primeiro jogador morreu, declare o segundo jogador vencedor
			al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");	//Indique o modo de conclusão do programa
			al_flip_display();																				//Atualiza a tela

			if ((event.type == 10) && (event.keyboard.keycode == 75))
                break;												                                        //Espera por um evento de teclado, de clique da tecla de espaço
			else if (event.type == 42) 
                break; 													                                    //Finaliza o jogo
		}
        
        else if(!enemies->ini){
            break; 													                                    //Finaliza o jogo
        }

		else{																								//Se nenhum quadrado morreu
			if (event.type == 30){		                                                                    //O evento tipo 30 indica um evento de relógio, ou seja, verificação se a tela deve ser atualizada (conceito de FPS)
				kill = update_position(player, enemies, obex);					                                    //Atualiza a posição dos objetos
                if(kill == 1)         					                    
                    player->life = 0;
				al_clear_to_color(al_map_rgb(0, 0, 0));														//Substitui tudo que estava desenhado na tela por um fundo preto
                al_draw_bitmap(playerBit, player->x-player->side_x/2, player->y-player->side_y/2, 0);       //Insere o bitmap do jogador na tela
                for(short int i = 0; i < player->life; i++)
                    al_draw_bitmap(lifeBit, i*36+4, 16, 0);       //Insere o bitmap do jogador na tela


                
                for(enemy_node* index = enemies->ini; index != NULL; index = (enemy_node*)index->next){
                    if(frame < 10){
                        if(index->type <=1)
                            al_draw_bitmap(enemyBit1, index->x-index->side_x/2, index->y-index->side_y/2, 0);       //Insere o bitmap do jogador na tela
                        else if(index->type>=3)
                            al_draw_bitmap(enemyBit3, index->x-index->side_x/2, index->y-index->side_y/2, 0);
                        else
                            al_draw_bitmap(enemyBit2, index->x-index->side_x/2, index->y-index->side_y/2, 0);
                    }
                    else if(frame > 9){
                        if(index->type <=1)
                            al_draw_bitmap(enemyBit1_2, index->x-index->side_x/2, index->y-index->side_y/2, 0);       //Insere o bitmap do jogador na tela
                        else if(index->type>=3)
                            al_draw_bitmap(enemyBit3_2, index->x-index->side_x/2, index->y-index->side_y/2, 0);
                        else
                            al_draw_bitmap(enemyBit2_2, index->x-index->side_x/2, index->y-index->side_y/2, 0);
                    }
                    if(frame == 19)
                        frame = 0;
                }
                for(obstacle* index = obex; index != NULL; index = (obstacle*)index->next){
                    if(index->life >=8)
                        al_draw_bitmap(bar1, index->x-index->side/2, index->y-index->side/2, 0);       //Insere o bitmap do jogador na tela
                    else if(index->life >=6)
                        al_draw_bitmap(bar2, index->x-index->side/2, index->y-index->side/2, 0);       //Insere o bitmap do jogador na tela
                    else if(index->life>=4)
                        al_draw_bitmap(bar3, index->x-index->side/2, index->y-index->side/2, 0);
                    else if(index->life>=2)
                        al_draw_bitmap(bar4, index->x-index->side/2, index->y-index->side/2, 0);
                    else if(index->life>=0)
                        al_draw_bitmap(bar5, index->x-index->side/2, index->y-index->side/2, 0);

                    }
                sprintf(sc, "%ld", score);
                frame++;
                al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN - 100, 15, 0, sc);
	    		for (bullet *index = player->gun->shots; index != NULL; index = (bullet*) index->next) 
                    al_draw_filled_circle(index->x, index->y, 4, al_map_rgb(255, 0, 0));			        //Insere as balas existentes disparadas pelo primeiro jogador na tela (!)
	    		if (player->gun->timer) 
                    player->gun->timer--;																	//Atualiza o cooldown da arma do primeiro jogador (!)
	    		

                for (bullet *index = enemies->gun->shots; index != NULL; index = (bullet*) index->next) 
                    al_draw_filled_circle(index->x, index->y, 4, al_map_rgb(0, 0, 255));			        //Insere as balas existentes disparadas pelo primeiro jogador na tela (!)
	    		if (enemies->gun->timer) 
                    enemies->gun->timer--;																	//Atualiza o cooldown da arma do primeiro jogador (!)
	    		
                al_flip_display();																			//Insere as modificações realizadas nos buffers de tela
			}
			else if ((event.type == 10) || (event.type == 12)){												//Verifica se o evento é de botão do teclado abaixado ou levantado
				if (event.keyboard.keycode == 1) 
                    joystick_left(player->control);							                                //Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à esquerda)
				else if (event.keyboard.keycode == 4) 
                    joystick_right(player->control);					                                    //Indica o evento correspondente no controle do primeiro jogador (botão de movimentação à direita)
				else if (event.keyboard.keycode == 75) 
                    joystick_fire(player->control);						                                    //Indica o evento correspondente no controle do primeiro joagdor (botão de disparo - c) (!)					
			}																																			
		 else if (event.type == 42) break;																//Evento de clique no "X" de fechamento da tela. Encerra o programa graciosamente.
	    }
	    }

        if(!enemies->ini){
			al_clear_to_color(al_map_rgb(0, 0, 0));															//Limpe a tela atual para um fundo preto
            al_draw_text(font, al_map_rgb(255, 0, 0), X_SCREEN/2 - 75, Y_SCREEN/2-15, 0, "FIM DE ESTAGIO!");	        //Se o segundo jogador morreu, declare o primeiro jogador vencedor
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+5, 0, "PRESSIONE ESPAÇO PARA SAIR");	//Indique o modo de conclusão do programa
            al_draw_text(font, al_map_rgb(255, 255, 255), X_SCREEN/2 - 110, Y_SCREEN/2+25, 0, "PRESSIONE C PARA NOVA RODADA");	//Indique o modo de conclusão do programa
        
			al_flip_display();																				//Atualiza a tela
			if ((event.type == 10) && (event.keyboard.keycode == 75))
                break;												                                        //Espera por um evento de teclado, de clique da tecla de espaço
			else if((event.type == 10) && (event.keyboard.keycode == 3)){
                enemies = enemy_destroy(enemies);
                enemies = enemy_create(X_SCREEN, Y_SCREEN);

            }
            else if (event.type == 42) 
                break; 													                                    //Finaliza o jogo


        }
        else
            break;
    }

	al_destroy_font(font);																				    //Destrutor da fonte padrão
	al_destroy_display(disp);																				//Destrutor da tela
	al_destroy_timer(timer);																				//Destrutor do relógio
	al_destroy_event_queue(queue);																			//Destrutor da fila
	player_destroy(player);																				    //Destrutor do quadrado do primeiro jogador
    enemies = enemy_destroy(enemies);
    destroy_obex(obex);

	return 0;
}

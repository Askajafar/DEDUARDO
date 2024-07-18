#include <stdlib.h>
#include "Player.h"

//Implementação da função "square_create"
player* player_create(unsigned char side_x, unsigned char side_y, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y){			
	if ((x - side_x/2 < 0) || (x + side_x/2 > max_x) || (y - side_y/2 < 0) || (y + side_y/2 > max_y)) 
        return NULL;										//Verifica se a posição inicial é válida; caso não seja, retorna NULL
	if (face > 3) 
        return NULL;								        //Verifica se a face principal do quadrado é válida (!)

	player *new_player = (player*) malloc(sizeof(player));	//Aloca memória na heap para um novo quadrado
	if (!new_player) return NULL;							//Se a alocação não deu certo, retorna erro													
	new_player->life = 3;
    new_player->side_x = side_x;							//Insere o tamanho do lado do player
    new_player->side_y = side_y;							//Insere o tamanho da altura do player
	new_player->face = face;								//Insere a indicação da face principal do quadrado (!)
	new_player->x = x;										//Insere a posição inicial central de X
	new_player->y = y;										//Insere a posição inicial central de Y
	new_player->control = joystick_create();				//Insere o elemento de controle do quadrado
	new_player->gun = pistol_create();						//Insere o elemento de disparos do quadrado (!)
	return new_player;										//Retorna o novo quadrado
}

//Implementação da função "player_move"
void player_move(player *element, char steps, unsigned char trajectory, unsigned short max_x, unsigned short max_y){							       
	if (!trajectory){ 
        if ((element->x - steps*PLAYER_STEP) - element->side_x/2 >= 0) 
            element->x = element->x - steps*PLAYER_STEP;
        else
            element->x = 0 + element->side_x/2;
    } 			                                                            //Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else if (trajectory == 1){ 
        if ((element->x + steps*PLAYER_STEP) + element->side_x/2 <= max_x) 
            element->x = element->x + steps*PLAYER_STEP;
        else
            element->x = max_x - element->side_x/2;
    }	                                                                    //Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
}

//Implementação da função "player_shot" (!)
void player_shot(player *element){																														
    bullet *shot;
    shot = pistol_shot(element->x, element->y - element->side_y/2, element->face, element->gun);		//Player atira para cima (!)

	if (shot) {
        shot->type = 3;
        element->gun->shots = shot;
    }
}

//Implementação da função "player_destroy"
void player_destroy(player *element){																														
	pistol_destroy(element->gun);		//Destrói o armemento do quadrado (!)
	joystick_destroy(element->control);	//Destrói o controle do quadrado
	free(element);						//Libera a memória do quadrado na heap
}

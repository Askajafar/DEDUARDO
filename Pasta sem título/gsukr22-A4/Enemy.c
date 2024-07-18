#include <stdlib.h>

#include "Enemy.h"

enemy_node *enemy_node_create(unsigned char side_x, unsigned char side_y, unsigned char face, unsigned short x, unsigned short y, unsigned short max_x, unsigned short max_y, unsigned char type){//Implementação da função "square_create"
	if ((x - side_x/2 < 0) || (x + side_x/2 > max_x) || (y - side_y/2 < 0) || (y + side_y/2 > max_y)) 
        return NULL;												//Verifica se a posição inicial é válida; caso não seja, retorna NULL
	if (face > 3) return NULL;										//Verifica se a face principal do quadrado é válida (!)

	enemy_node *new_enemy = (enemy_node*) malloc(sizeof(enemy_node));				//Aloca memória na heap para um novo quadrado
	if (!new_enemy)
        return NULL;								//Se a alocação não deu certo, retorna erro													
    
    new_enemy->type = type;
	new_enemy->side_x = side_x;										//Insere o tamanho do lado do player
    new_enemy->side_y = side_y;										//Insere o tamanho da altura do player
	new_enemy->face = face;											//Insere a indicação da face principal do quadrado (!)
	new_enemy->x = x;												//Insere a posição inicial central de X
	new_enemy->y = y;												//Insere a posição inicial central de Y
	new_enemy->life = 1;
    return new_enemy;												//Retorna o novo quadrado
}

enemy *enemy_create(unsigned short max_x, unsigned short max_y){//Implementação da função "square_create"
	size_t enemy_x, enemy_y, qt, k = 0;
    unsigned char type = 1, side_x;
    enemy *new_enemy = NULL;
    enemy_node *aux = NULL;

    if(max_x - 170 < 0)
        return NULL;
    if(max_y - 200 < 0)
        return NULL;
    enemy_x = (max_x - 170) / 70;
    enemy_y = (max_y - 500) / 54;
    qt = enemy_y /3;

	if(!(new_enemy = (enemy*) malloc(sizeof(enemy))))
        return NULL;
    new_enemy->dir = 1;												//Insere o elemento de controle do quadrado
	new_enemy->gun = pistol_create();								//Insere o elemento de disparos do quadrado (!)
    new_enemy->gun->timer = 30;
    new_enemy->ini = NULL;


    for(int j = 0; j< enemy_y; j++){
        for(int i = 0; i < enemy_x; i++){
	        aux = new_enemy->ini;
            if(type<=1)
                side_x = 36;
            else if(type >=3)
                side_x = 48;
            else
                side_x = 44;
            new_enemy->ini = enemy_node_create(side_x, 32, 1, (i*70 + 70), (j * 54 + 16), max_x, max_y, type);
            if (!new_enemy)
                return NULL;						//Se a alocação não deu certo, retorna erro													
            new_enemy->ini->next = (struct enemy_node*) aux;
        }
        if(k>=qt){
            if(type < 3)
                type = type + 1;
            k = 0;
        }
        else 
            k++;
    }
	return new_enemy;												//Retorna o novo quadrado
}

unsigned short enemy_down(enemy_node *element, unsigned short max_y){
    if ((element->y + 20) + element->side_y/2 <= max_y-32) {
        element->y = element->y + 20;
        return 0;
    }
    else{
        return 1;
    }
}

unsigned char enemy_move(enemy *element, char steps, unsigned short max_x, unsigned short max_y){							//Implementação da função "square_move"
    unsigned char  kill = 0;
	if (!element->dir){
        for(enemy_node *aux = element->ini; aux!=NULL; aux = (enemy_node*)aux->next){
            if (((aux->x - steps*ENEMY_STEP) - aux->side_x/2 < 0))
                element->dir = 1;
        }

        if(!element->dir)
            for(enemy_node *aux = element->ini; aux!=NULL; aux = (enemy_node*)aux->next)
                aux->x = aux->x - steps*ENEMY_STEP;
        else{
            for(enemy_node *aux = element->ini; aux!=NULL; aux = (enemy_node*)aux->next){
                kill = enemy_down(aux, max_y);
                if(kill)
                    return kill;
            }
        }
        
    } 			//Verifica se a movimentação para a esquerda é desejada e possível; se sim, efetiva a mesma
	else{ 
        for(enemy_node *aux = element->ini; aux!=NULL; aux = (enemy_node*)aux->next){
            if ((aux->x + steps*ENEMY_STEP) + aux->side_x/2 > max_x) 
                element->dir = 0;
        }
        if(element->dir)
            for(enemy_node *aux = element->ini; aux!=NULL; aux = (enemy_node*)aux->next)
                aux->x = aux->x + steps*ENEMY_STEP;
        else{
            for(enemy_node *aux = element->ini; aux!=NULL; aux = (enemy_node*) aux->next){
                kill = enemy_down(aux, max_y);
                if(kill)
                    return kill;
            }
        }

    }	//Verifica se a movimentação para a direita é desejada e possível; se sim, efetiva a mesma
    return kill;
}

void enemy_shot(enemy* element, enemy_node *node){
    bullet *shot;
    shot = pistol_shot(node->x, node->y + node->side_y, node->face, element->gun);    //Quadrado atira para baixo (!)
	if (shot){
        if(node->type>3)
            shot->type = 3;
        else 
            shot->type = 1;

        element->gun->shots = shot;
    }
}

enemy* enemy_destroy(enemy *enemies){
    enemy_node *aux = NULL, *element = (enemy_node*)enemies->ini;
    pistol_destroy(enemies->gun);
    while (element!=NULL){
        aux = (enemy_node*) element->next;
        free(element);
        element = aux;
    }
    free(enemies);
    return NULL;
}

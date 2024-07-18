#ifndef __ENEMY__
#define __ENEMY__


#define ENEMY_STEP 5

#include "Pistol.h"

typedef struct {
    unsigned char type;
	unsigned char side_x;	//Tamanmho da lateral do inimigo
	unsigned char side_y;	//Tamanmho da altura do player
	unsigned char face;		//A face principal do inimigo, a direcao do disparo do inimigo(!)
	unsigned short x;		//Posição X do centro do inimigo
	unsigned short y;		//Posição Y do centro do inimigo
    unsigned char life;
    struct enemy_node *next;
} enemy_node;

typedef struct {			//Definição da estrutura de um inimigo
    pistol *gun;
	short int dir;          //direcao de movimento do inimigo
    enemy_node *ini;
} enemy;					//Definição do nome da estrutura


enemy *enemy_create(unsigned short max_x, unsigned short max_y);
unsigned char enemy_move(enemy *element, char steps, unsigned short max_x, unsigned short max_y);
void enemy_shot(enemy *element, enemy_node *node);
enemy* enemy_destroy(enemy *element); 



#endif

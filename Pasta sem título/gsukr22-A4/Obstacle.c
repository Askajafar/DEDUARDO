#include <stdlib.h>
#include "Obstacle.h"

obstacle *create_node_obex(unsigned short x, unsigned short y,unsigned char side){
    obstacle *obex;
    if(!(obex = (obstacle*)malloc(sizeof(obstacle))))
        return NULL;
    obex->x = x;
    obex->y = y;
    obex->side = side;
    obex->life = 10;
    return obex;
}

obstacle *create_obex(unsigned short max_x, unsigned short max_y, unsigned char side){
    size_t obex_x;
    obstacle *obex = NULL, *aux = NULL;

    obex_x = (max_x-32)/180;
    
    for(size_t i = 0; i<obex_x; i++){
        aux = obex;
        obex = create_node_obex(i*180+90,max_y - 68, side);
        obex->next = (struct obstacle*)aux;
        aux = obex;
        obex = create_node_obex(i*180+90,max_y - 100, side);
        obex->next = (struct obstacle*)aux;
        aux = obex;
        obex = create_node_obex(i*180+122,max_y - 100, side);
        obex->next = (struct obstacle*)aux;
        aux = obex;
        obex = create_node_obex(i*180+154,max_y - 100, side);
        obex->next = (struct obstacle*)aux;
        aux = obex;
        obex = create_node_obex(i*180+154,max_y - 68, side);
        obex->next = (struct obstacle*)aux;
    }
    return obex;
}

void destroy_obex(obstacle *element){
    obstacle *aux = element;																			//Sentinela para a remoção de projéteis ativos (!)
	while(aux != NULL){								//Para cada projétil na lista de disparos (!)
		aux = (obstacle*)aux->next;
        free(element);
        element = aux;																//Armazena o próximo projétil (!)																			//Chama o destrutor do projétil atual (!)
	}
																						//Libera a memória da instância de pistola (!)
}


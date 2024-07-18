#include <stdlib.h>
#include "Bullet.h"
#include <stdio.h>

//Implementação da função "bullet_create"
bullet* bullet_create(unsigned short x, unsigned short y, unsigned char trajectory, bullet *next){		
	if ((trajectory < 0) || (trajectory > 1)) return NULL;	//Verifica se a trajetória informada para o projétil é válida

	bullet *new_bullet = (bullet*) malloc(sizeof(bullet));	//Aloca memória na heap para uma instância de projétil
	if (!new_bullet)
        return NULL;							//Verifica o sucesso da alocação de memória; retorna NULL em caso de falha
	new_bullet->x = x;										//Armazena a posição X informada
	new_bullet->y = y;										//Armazena a posição Y informada
	new_bullet->trajectory = trajectory;					//Armazena a trajetória informada
	new_bullet->next = (struct bullet*) next;				//Armazena o próximo projétil informado
	return new_bullet;										//Retorna a instância criada de projétil
}

//Implementação da função "bullet_move"
bullet *bullet_move(bullet *element, unsigned short max_y){																			
	bullet *previous = NULL, *index;
    index = element;
    if(index == NULL)
        return NULL;
    while(index != NULL){	//Para cada projétil presente na lista de projéteis informada
        if((index->y-BULLET_MOVE <= 0)|| (index->y+BULLET_MOVE >= max_y)){//Verifica se o projétil saiu das bordas da janela (!)				                            			        
            if (previous!=NULL){											//Verifica se não é o primeiro elemento da lista de projéteis (!)
		        previous->next = index->next;						//Se não for, salva o próximo projétil (!)
			    bullet_destroy(index);								//Chama o destrutor para o projétil atual (!)
			    index = (bullet*) previous->next;					//Atualiza para o próximo projétil (!)
	        }
			else {
		        element = (bullet*) index->next;			//Atualiza o projétil no início da lista (!)
			    bullet_destroy(index);								//Chama o destrutor para o projétil atual (!)
		        index = element;							//Atualiza para o próximo projétil (!)
	        }
        }
        else{
            previous = index;
            index = (bullet*) index->next;
            if (!previous->trajectory)
                previous->y = previous->y - BULLET_MOVE;										//Se a trajetória for para a esquerda, movimenta um passo à esquerda
            else
                previous->y = previous->y + BULLET_MOVE;	
        }
    }
    return element;
}

void bullet_destroy(bullet *element){		//Implementação da função "bullet_destroy"
    if(element!=NULL)
	    free(element);							//Libera a memória da instância de projétil
}

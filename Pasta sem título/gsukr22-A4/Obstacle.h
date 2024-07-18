#ifndef __OBSTACLE__
#define __OBSTACLE__


typedef struct{
    int life;
    unsigned short x;
    unsigned short y;
    unsigned char side;
    struct obstacle *next;
}obstacle;

obstacle *create_obex(unsigned short max_x, unsigned short max_y, unsigned char side);

void destroy_obex(obstacle *element);
#endif

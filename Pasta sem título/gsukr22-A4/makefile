alg = $(shell pkg-config allegro-5 allegro_main-5 allegro_font-5 allegro_primitives-5 allegro_image-5 --libs --cflags)
CC = gcc -Wall 

obj = SpaceInvaders.o Player.o Joystick.o Pistol.o Bullet.o Enemy.o Obstacle.o

AS: $(obj)
	$(CC) $(obj) -o AS $(alg)
	
SpaceInvaders.o: SpaceInvaders.c
	$(CC) -c SpaceInvaders.c

Player.o: Player.c
	$(CC) -c Player.c

Joystick.o: Joystick.c
	$(CC) -c Joystick.c

Pistol.o: Pistol.c
	$(CC) -c Pistol.c

Bullet.o: Bullet.c
	$(CC) -c Bullet.c

Enemy.o: Enemy.c
	$(CC) -c Enemy.c

Obstacle.o: Obstacle.c
	$(CC) -c Obstacle.c


clean: 
	rm AS $(obj)

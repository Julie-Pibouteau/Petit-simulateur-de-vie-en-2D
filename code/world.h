#ifndef WORLD_H
#define WORLD_H

int** init_world(int h, int w);

/*ajoute 1 nourriture dans le monde et 1 dans la beauce si elle existe*/
void new_food(int** World,int h, int w, int xB, int yB, int hB, int wB);

void display_world(int** World, int h, int w);

void free_world(int** World, int h);

#endif

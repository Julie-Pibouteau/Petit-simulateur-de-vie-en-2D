#include "animal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


animal* init_animal(int* chromosome,int energy_level, int direction, int x, int y, int family) {
  animal *new_animal = malloc(sizeof(animal));
  if(new_animal==NULL) return NULL;
  new_animal->chromosome = chromosome; // !! le pointeur pointera sur le tab chromosome que nous allons lire depuis le fichier .phine --> ne pas oublier de faire un malloc àce moment !
  new_animal->energy_level = energy_level;
  new_animal->direction = direction;
  new_animal->x = x;
  new_animal->y = y;
  new_animal->family = family;
  return new_animal;
}

void display_animal(animal* a) {
  printf("(%d %d) %d %d | %d %d %d %d %d %d %d %d|\n", a->x, a->y, a->direction, a->energy_level, a->chromosome[0], a->chromosome[1], a->chromosome[2], a->chromosome[3], a->chromosome[4], a->chromosome[5], a->chromosome[6], a->chromosome[7]);
}

void write_animal(FILE* f_out,animal* a) {
  fprintf(f_out, "(%d %d) %d %d | ", a->x, a->y, a->direction, a->energy_level);
  for(int i = 0; i < 8; i++) {
    fprintf(f_out, "%d ", a->chromosome[i]);
  }
  fprintf(f_out, "|\n");
}

void free_animal(animal* a) {
  free(a->chromosome);
  free(a);
}

void orientation(animal *a) {
  int i, x;
  int cumulative_headcount[8] = {a->chromosome[0],0,0,0,0,0,0,0};
  for(i=1; i < 8; i++) {
    cumulative_headcount[i]+= a->chromosome[i] + cumulative_headcount[i-1];
  }
  x = rand()%(cumulative_headcount[7]+1);

  i = 0;
  while (x > cumulative_headcount[i]){
    i++;
  }
  a->direction = (i+a->direction)%8;
}

void move(animal* a, int h_World, int w_World){
  //On notera que les coordonnées (x,y) = World[y][x].

  switch(a->direction) {

    case 0:
      (a->y)+=1;
      (a->y) = (a->y)%h_World;
      break;
      // en dessous

    case 1:
      (a->y)+=1;
      (a->y) = (a->y)%h_World;
      (a->x)+=1;
      (a->x) = (a->x)%w_World;
      break;

    case 2:
      (a->x)+=1;
      (a->x) = (a->x)%w_World;
      break;
      // à droite

    case 3:
      (a->y)+=h_World-1;
      (a->y) = (a->y)%h_World;
      (a->x)+=1;
      (a->x) = (a->x)%w_World;
      break;

    case 4:
      (a->y)+=h_World-1;
      (a->y) = (a->y)%h_World;
      break;
      // au dessus

    case 5:
      (a->y)+=h_World-1;
      (a->y) = (a->y)%h_World;
      (a->x)+=w_World-1;
      (a->x) = (a->x)%w_World;
      break;

    case 6:
      (a->x)+=w_World-1;
      (a->x) = (a->x)%w_World;
      break;
      // à gauche

    case 7:
      (a->x)+=w_World-1;
      (a->x) = (a->x)%w_World;
      (a->y)+=1;
      (a->y) = (a->y)%h_World;
      break;
  }
}

float distance_animal_Beauce(animal* a, float x_Bcenter, float y_Bcenter){
  float diff_x = x_Bcenter - (a->x);
  if (diff_x < 0) diff_x = -diff_x;
  float diff_y = y_Bcenter - (a->y);
  if (diff_y < 0) diff_y = -diff_y;

  float distance;
  if (diff_x > diff_y) distance = diff_x;
  else distance = diff_y;
  return distance;
}

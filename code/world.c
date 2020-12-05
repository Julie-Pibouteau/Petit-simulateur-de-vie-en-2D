#include"world.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int** init_world(int h, int w){
  int** World = malloc(h*sizeof(int*));
  int i,j;
  for(i=0; i<h; i++){
    World[i] = malloc(w*sizeof(int));
    for(j=0; j<w; j++){
      World[i][j] = 0;
    }
  }
  return World;
}


void new_food(int** World,int h, int w, int xB, int yB, int hB, int wB){
  int h_alea = rand() % h;
  int w_alea = rand() % w;
  (World[h_alea][w_alea]) ++;
  //si beauce :
  if(hB!=0 && wB!=0){
    h_alea = yB + (rand() % hB);
    w_alea = xB + (rand() % wB);
    (World[h_alea][w_alea]) ++;

  }
}


void display_world(int** World, int h, int w){
  int i,j;
  for(i=0; i<h; i++){
    for(j=0; j<w; j++){
      printf("%d ", World[i][j]);
    }
    printf("\n");
  }
}

void free_world(int** World, int h){
  int i;
  for(i=0; i<h; i++){
    free(World[i]);
    }
  free(World);
}

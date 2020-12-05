#ifndef ANIMAL_H
#define ANIMAL_H
#include <stdio.h>

typedef struct {
  int *chromosome; 
  int energy_level;
  int direction;
  int x;
  int y;
  int family;
} animal;

/* Initialise un animal */
animal* init_animal(int* chromosome, int energy_level, int direction, int x, int y, int family);

/* Affiche l'animal */
void display_animal(animal* a);

/* écrit dans le fichier f_out les informations de l'animal a */
void write_animal(FILE* f_out,animal* a);


void free_animal(animal* a);

// génère la direction du prochain déplacement de l'animal
void orientation(animal *a);

// déplace l'animal APRES avoir décidé de sa direction
void move(animal* a, int h_World, int w_World);

// donner la distance entre un animal et le centre de la Beauce
float distance_animal_Beauce(animal* a, float x_Bcenter, float y_Bcenter);

#endif

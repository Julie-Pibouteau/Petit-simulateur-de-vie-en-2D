#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include"animal.h"

typedef struct cell{
  animal* a;
  struct cell* next;
  struct cell* prev;
} cell;

typedef struct {
  cell* start;
} list;



list* init_list(void);

/* ajoute une cellule contenant l'animal a au début de la liste l */
void add_start(list* l, animal* a);

/* affiche tous les animaux de la liste l */
void display_list(list* l);

/* ecrit dans le fichier f_out la liste d'animaux l */
void write_list(FILE* f_out, list* l);

/* enlève la CELLULE c de la liste l et libere l'espace de CELLULE
ATTENTION ça ne libère pas l'animal contenu dans la cellule (pour pouvoir garder l'animal dans une autre liste par exemple)*/
void delete_cell(list* l, cell* c);

/* libère toutes les cellules de la liste l et la liste l elle-même
ATTENTION ça ne libère pas les animaux contenus dans les cellules, juste les cellules (pour pouvoir garder l'animal dans une autre liste par exemple)*/
void delete_list(list* l);

/* libere toutes les cellules, animaux, chromosomes contenus dans la liste et la liste elle-même */
void free_list(list* l);

// duplique un animal lorsque son niveau d'énergie dépasse le seuil de reproduction
animal* reproduction(animal*a, list*family_array[], list*global_list);

#endif

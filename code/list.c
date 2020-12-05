#include "list.h"
#include <stdio.h>
#include <stdlib.h>

list* init_list(void){
  list* l = malloc(sizeof(list));
  if (l==NULL)
    return NULL;
  l->start = NULL;
  return l;
}

void add_start(list* l, animal* a){
  cell* c = malloc(sizeof(cell));
  if (c==NULL){
    return;
  }
  c->a = a;
  if(l->start != NULL)
    (l->start)->prev = c;
  c->next = l->start;
  l->start = c;
  c->prev = NULL;
}

void display_list(list* l){
  cell* tmp = l->start;
  while(tmp!=NULL){
    display_animal(tmp->a);
    tmp = tmp->next;
  }
}

void write_list(FILE* f_out, list* l){
  cell* tmp = l->start;
  while(tmp!=NULL){
    write_animal(f_out, tmp->a);
    tmp = tmp->next;
  }
}

void delete_cell(list* l, cell* c){
  if(c->next != NULL)
    (c->next)->prev = c->prev;
  if(c->prev == NULL)
    l->start = c->next ;
  else
    (c->prev)->next = c->next;
  free(c);
}

void delete_list(list* l){
  if(l->start ==NULL){
    free(l);
    return;
  }
  delete_cell(l, l->start);
  delete_list(l);
}

void free_list(list* l){
  if(l->start == NULL){
    free(l);
    return;
  }
  free_animal(l->start->a);
  delete_cell(l, l->start);
  free_list(l);
}

animal* reproduction(animal*a, list* family_array[], list*global_list){

  int* new_chromosome = malloc(8*sizeof(int));
  for(int i=0; i<8; i++){
    new_chromosome[i] = a->chromosome[i];
  }
  //si le niveau d'énergie du parent est impair, on donne la partie entière supérieure de la moitié de son énergie à son enfant, et la partie entière inférieure de la moitié de son énergie au parent
  animal* new_animal = init_animal(new_chromosome,(a->energy_level)/2+(a->energy_level)%2,a->direction, a->x, a->y, a->family);
  a->energy_level /= 2;
  // genere le numero du gene à changer
  int gene_number = rand()%8;
  // genere la valeur de la mutation
  int mutation_value = (rand()%3)-1;
  if (new_animal->chromosome[gene_number] > 1 || mutation_value!=-1) {
    new_animal->chromosome[gene_number]+=mutation_value;
  }
  add_start(family_array[a->family],new_animal); // ajouter le nouvel animal à sa famille
  add_start(global_list, new_animal); // ajouter le nouvel animal à la liste de tous les animaux
  return new_animal;
}

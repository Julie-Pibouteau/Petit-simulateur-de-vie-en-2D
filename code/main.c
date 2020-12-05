#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "world.h"
#include "list.h"
#include "animal.h"
#define N 200

/* affiche un message d'erreur dans un fichier Erreur et rappelle la syntaxe d'execution */
void usage(char *prog) {
    fprintf(stderr, "Usage: %s r n input.phine output.phine \n\n\tr: 0 (without randomness) or 1 (with randomness)\n\tn: number of iterations\n", prog);
}

void remove_comments(char line[]){
  for(int i=0; i<strlen(line); i++){
    if(line[i]=='#'){
      line[i]='\0';
      return;
    }
  }
}

/* fonction qui enlève les espaces et tabulations au debut d'une chaîne passée en argument */
void remove_first_spaces(char line[]){
  int len = strlen(line);
  int i,j;
  for(i=0; i<len && (line[i]==' ' || line[i]=='\t'); i++);
  for(j=0; j<len-i+1; j++){
    line[j]=line[i+j];
  }
}


int main(int argc, char** argv) {
   int nb_iterations = 0;
   char* input;
   char* output;
   int r;

/* vérifie qu'il y a 5 ou 6 arguments --> sinon affiche un message d'erreur dans un fichier stderr et arrête le programme*/
   if(argc<5 || argc>6) {
      usage(argv[0]);
      return 0;
   }
/* affecte l'argument à r ET vérifie que r est 0 ou 1*/
   if(sscanf(argv[1], "%d", &r)!=1 || (r != 1 && r!=0)) {
      fprintf(stderr, "incorrect value for r (%s)\n", argv[1]);
      usage(argv[0]);
      return 0;
   }
/* affecte l'argument à n ET vérifie que n est un entier positif*/
   if(sscanf(argv[2], "%d", &nb_iterations)!=1 || nb_iterations<0) {
      fprintf(stderr, "incorrect value for n (%s)\n", argv[2]);
      usage(argv[0]);
      return 0;
   }
/* affecte les fichiers passés en arguments à input et output*/
   input=argv[3];
   output=argv[4];

   if(r) {
      srand(time(NULL));
   }



/* DEBUT LECTURE FICHIER */
  FILE* f_input = fopen(input, "r");
  if(f_input == NULL){
    fprintf(stderr, "%s : input file error (%s)\n", argv[0], input);
    return 0;
  }

  char line[N];
  int h_world, w_world, x_beauce, y_beauce, h_beauce, w_beauce, energy_rise, reprod_level;
  int continue_ = 1;
  int verif[4]={0};

  while(continue_){
    if(fgets(line, N, f_input)==NULL){
      fprintf(stderr, "%s : Erreur fichier incomplet (ou aucun animal) ", argv[0]);
      return 0;
    }
    remove_comments(line);
    remove_first_spaces(line);
    switch(line[0]) {
      case '#':
        break;

      case 'M':
        if(sscanf(line,"Monde %d %d", &h_world, &w_world) != 2 || h_world<0 || w_world<0){
          fprintf(stderr, "%s :Erreur donnees du Monde\n",argv[0]);
          return 0;
        }
        verif[0]=1;
        break;

      case 'B':
        if(sscanf(line,"Beauce %d %d %d %d", &x_beauce, &y_beauce, &h_beauce, &w_beauce) != 4 || x_beauce<0 || y_beauce<0 || h_beauce<0 || w_beauce<0){
          fprintf(stderr, "%s : Erreur donnees de la Beauce\n", argv[0]);
          return 0;
        }
        if(y_beauce+h_beauce > h_world || x_beauce+w_beauce > w_world){
          fprintf(stderr,"%s : Erreur Beauce trop grande\n",argv[0]);
          return 0;
        }
        verif[1]=1;
        break;

      case 'E':
        if(sscanf(line,"Energie Nourriture %d", &energy_rise) != 1  || energy_rise<0){
          fprintf(stderr,"%s : Erreur Energie Nourriture\n",argv[0]);
          return 0;
        }
        verif[2]=1;
        break;

      case 'S':
        if(sscanf(line,"Seuil Reproduction %d", &reprod_level) != 1 || reprod_level<0){
          fprintf(stderr, "%s : Erreur Seuil Reproduction\n", argv[0]);
          return 0;
        }
        verif[3]=1;
        break;

      case '(':
        continue_ = 0;
        break;

      default :
        if(line[0] != '\n' && line[0]!='\0'){
          fprintf(stderr, "%s : Erreur ligne non reconnue/illisible dans le premier paragraphe.", argv[0]);
          return 0;
        }
    }
  }

  for(int i=0; i<4; i++){
    if(verif[i]!=1){
      fprintf(stderr, "%s : Erreur pas assez d'elements dans le fichier %s (Monde, Beauce, Energie Nourriture, Seuil Reproduction)\n", argv[0], argv[3]);
      return 0;
    }
  }

/* CREATION DU MONDE */
  int** World = init_world(h_world, w_world);

/* LECTURE DES DONNEES DES ANIMAUX */
  list* global_list = init_list();

/* on implémente les animaux */
  int cpt_family = 0;
  do {
  /* gestion des éventuels commentaires et espaces */
    if(line[0]=='#')
      continue;
    remove_comments(line);
    remove_first_spaces(line);
  /* création de l'animal */
    int* ch = malloc(8*sizeof(int));
    animal* a = init_animal(ch,0,0,0,0,cpt_family);
    if(sscanf(line, "(%d %d) %d %d | %d %d %d %d %d %d %d %d |", &(a->x), &(a->y), &(a->direction), &(a->energy_level), &(a->chromosome[0]), &(a->chromosome[1]), &(a->chromosome[2]), &(a->chromosome[3]), &(a->chromosome[4]), &(a->chromosome[5]), &(a->chromosome[6]), &(a->chromosome[7])) != 12){
      fprintf(stderr, "%s : Erreur lecture animal numero %d\n",argv[0],cpt_family+1);
      return 0;
    }
    if(a->x >= w_world || a->y >= h_world || a->x <0 || a->y <0){
      fprintf(stderr, "%s : Erreur lecture animal numero %d (coordonnees hors du monde)\n",argv[0],cpt_family+1);
      return 0;
    }
    add_start(global_list, a);
    cpt_family ++;
  } while(fgets(line, N, f_input)!=NULL && line[0]!='\n');

/* on crée le tableau contenant les familles d'animaux */
  list** family_array = calloc(cpt_family, sizeof(list));
  cell* tmp_cell = global_list->start;
  while(tmp_cell != NULL){
    list* l = init_list();
    add_start(l, tmp_cell->a);
    family_array[tmp_cell->a->family] = l;
    tmp_cell = tmp_cell->next;
  }

  fclose(f_input);




/* DEBUT DE L'EXPERIENCE : ITERATIONS DE TEMPS */
cell* p = NULL;
animal* tmp_a;
cell* tmp_p;
cell* tmp_next;

for(int n = 0; n < nb_iterations; n++){
  if( (n%100000) == 0)
    printf("%d iterations effectuees\n",n);
  // ajouter de la nourriture dans le monde
  new_food(World, h_world, w_world, x_beauce, y_beauce, h_beauce, w_beauce);
  // parcourir la liste de tous les animaux
  p = global_list->start;
  while(p != NULL){
    orientation(p->a);
    move(p->a, h_world, w_world);
    p->a->energy_level-=1;
    // on teste si la cellule contient de la nourriture
    if (World[p->a->y][p->a->x] >= 1){
      // Rq : l'animal gagne une unité de nourriture !=1 unit d'energie ! Il obtient plusieurs unités d'énergie.
      (p->a->energy_level)+=energy_rise;
      // mais le monde perd une unité d'énergie sur cette case donc -1.
      (World[p->a->y][p->a->x])--;
    }
    tmp_next = p->next;

    // tuer l'animal = le supprimer de la liste de tous les animaux et de celle de sa famille
    if(p->a->energy_level <= 0){
      // trouver l'animal dans sa famille
      tmp_cell = family_array[p->a->family]->start;
      while(tmp_cell->a != p->a){
        tmp_cell = tmp_cell->next;
      }


      //maintenant on peut supprimer
      tmp_a = tmp_cell->a ;
      tmp_p = p;
      delete_cell(family_array[tmp_cell->a->family], tmp_cell);
      free_animal(tmp_a);
      delete_cell(global_list, tmp_p);
    }
    else if(p->a->energy_level >= reprod_level){
        // duplication du parent(energy_level /2), creation d'un nouvel animal.
        reproduction(p->a, family_array, global_list);
    }

    p = tmp_next;
  }
}



/* FIN DE L'EXPERIENCE : TRI DES ANIMAUX PAR RAPPORT A LEUR DISTANCE A LA BEAUCE ET ECRITURE DANS LE FICHIER*/

  FILE* f_output = fopen(output, "w");
  fprintf(f_output, "Monde %d %d\nBeauce %d %d %d %d\nEnergie Nourriture %d\nSeuil Reproduction %d\n\n", h_world, w_world, x_beauce, y_beauce, h_beauce, w_beauce, energy_rise, reprod_level);

  /*Si la Beauce existe, on va écrire les animaux selon leur distance au centre de la Beauce, du plus proche au plus éloigné
  On commence par déterminer le centre de la Beauce.
  On considère les coordonnées exactes du centre, même si elles sont flottantes, pour être plus précis.
  */
  if (h_beauce != 0 && w_beauce != 0){
    float x_Bcenter = (float)x_beauce + (float)(w_beauce-1)/2;
    float y_Bcenter = (float)y_beauce + (float)(h_beauce-1)/2;

    cell* min = global_list->start;
    cell* current = global_list->start;
    float distance, distance_min = 0;
    //tant que la liste de tous les animaux n'est pas vide
    while (global_list->start != NULL){
      //chercher le min
      while (current != NULL){
        distance = distance_animal_Beauce(current->a, x_Bcenter, y_Bcenter);
        distance_min = distance_animal_Beauce(min->a, x_Bcenter, y_Bcenter);
        if(distance < distance_min){
          min = current;
        }
        current = current->next;
      }

      // écrire les données de l'animal à la plus petite distance à la Beauce dans le fichier de sortie
      write_animal(f_output, min->a);
      //puis supprimer la cellule contenant initialement cet animal, sans supprimer l'animal
      delete_cell(global_list, min);
      min = global_list->start;
      current = global_list->start;
    }

  }
  else{
    //si la Beauce n'existe pas, on écrit global_list telle quelle
    write_list(f_output, global_list);
  }
  fprintf(f_output, "\n\n");
  //enfin on écrit les animaux par familles
  for(int i=0; i<cpt_family; i++){
    if(family_array[i]->start != NULL){
      //fprintf(f_output, "famille numéro %d\n", i);
      write_list(f_output, family_array[i]);
      fprintf(f_output, "\n");
    }
  }
  fclose(f_output);



/* DESALLOCATION DE LA MEMOIRE */
  delete_list(global_list);
  for(int i=0; i<cpt_family; i++){
    free_list(family_array[i]);
  }
  free_world(World, h_world);
  free(family_array);

   return 0;
}

#ifndef GRILLE_H
#define GRILLE_H

#include "robot.h"

typedef struct {
    int taille;
    int **cellules;
} Grille;

void initialiser_grille(Grille *g, int taille);
void liberer_grille(Grille *g);
void afficher_grille(Grille *g, Robot *robots, int nb_robots);
int est_cellule_libre(Grille *g, int x, int y, Robot *robots, int nb_robots, int robot_id);
void placer_robot(Grille *g, int x, int y, int robot_id);
void retirer_robot(Grille *g, int x, int y);

#endif

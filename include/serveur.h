#ifndef SERVEUR_H
#define SERVEUR_H

#include "robot.h"
#include "grille.h"

typedef struct {
    int *new_positions;
    int nb_robots;
} Serveur;

void initialiser_serveur(Serveur *s, int nb_robots);
void liberer_serveur(Serveur *s);
void coordonner_robots(Serveur *s, Robot *robots, Grille *g, int nb_robots);
int detecter_collision(int *positions, int nb_robots, int robot_id);

#endif

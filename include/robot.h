#ifndef ROBOT_H
#define ROBOT_H

typedef struct {
    int id;
    int x;
    int y;
    int cible_x;
    int cible_y;
    int actif;
} Robot;

void initialiser_robot(Robot *r, int id, int x, int y, int cible_x, int cible_y);
void calculer_prochain_deplacement(Robot *r, int *new_x, int *new_y);
int robot_atteint_cible(Robot *r);
void afficher_robot(Robot *r);

#endif

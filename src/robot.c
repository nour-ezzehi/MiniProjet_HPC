#include "robot.h"
#include <stdio.h>
#include <stdlib.h>

void initialiser_robot(Robot *r, int id, int x, int y, int cible_x, int cible_y) {
    r->id = id;
    r->x = x;
    r->y = y;
    r->cible_x = cible_x;
    r->cible_y = cible_y;
    r->actif = 1;
}

void calculer_prochain_deplacement(Robot *r, int *new_x, int *new_y) {
    *new_x = r->x;
    *new_y = r->y;
    
    if (!r->actif) return;
    
    // Déplacement vers la cible (Manhattan)
    if (r->x < r->cible_x) {
        (*new_x)++;
    } else if (r->x > r->cible_x) {
        (*new_x)--;
    } else if (r->y < r->cible_y) {
        (*new_y)++;
    } else if (r->y > r->cible_y) {
        (*new_y)--;
    }
}

int robot_atteint_cible(Robot *r) {
    return (r->x == r->cible_x && r->y == r->cible_y);
}

void afficher_robot(Robot *r) {
    printf("Robot %d: Position(%d,%d) -> Cible(%d,%d) [%s]\n",
           r->id, r->x, r->y, r->cible_x, r->cible_y,
           r->actif ? "actif" : "inactif");
}

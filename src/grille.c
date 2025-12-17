#include "grille.h"
#include <stdio.h>
#include <stdlib.h>

void initialiser_grille(Grille *g, int taille) {
    g->taille = taille;
    g->cellules = (int **)malloc(taille * sizeof(int *));
    for (int i = 0; i < taille; i++) {
        g->cellules[i] = (int *)calloc(taille, sizeof(int));
    }
}

void liberer_grille(Grille *g) {
    for (int i = 0; i < g->taille; i++) {
        free(g->cellules[i]);
    }
    free(g->cellules);
}

void afficher_grille(Grille *g, Robot *robots, int nb_robots) {
    printf("\n");
    for (int i = 0; i < g->taille; i++) {
        for (int j = 0; j < g->taille; j++) {
            int robot_id = -1;
            int cible_id = -1;

            // Chercher si un robot est sur cette case
            for (int k = 0; k < nb_robots; k++) {
                if (robots[k].x == i && robots[k].y == j && robots[k].actif) {
                    robot_id = robots[k].id;
                    break;
                }
            }

            // Chercher si c'est une cible
            for (int k = 0; k < nb_robots; k++) {
                if (robots[k].cible_x == i && robots[k].cible_y == j && robots[k].actif) {
                    cible_id = robots[k].id;
                    break;
                }
            }

            if (robot_id >= 0) {
                printf("%d", robot_id);
            } else if (cible_id >= 0) {
                printf("* ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

int est_cellule_libre(Grille *g, int x, int y, Robot *robots, int nb_robots, int robot_id) {
    if (x < 0 || x >= g->taille || y < 0 || y >= g->taille) {
        return 0;
    }

    for (int i = 0; i < nb_robots; i++) {
        if (i != robot_id && robots[i].actif) {
            if (robots[i].x == x && robots[i].y == y) {
                return 0;
            }
        }
    }
    return 1;
}

void placer_robot(Grille *g, int x, int y, int robot_id) {
    if (x >= 0 && x < g->taille && y >= 0 && y < g->taille) {
        g->cellules[x][y] = robot_id + 1;
    }
}

void retirer_robot(Grille *g, int x, int y) {
    if (x >= 0 && x < g->taille && y >= 0 && y < g->taille) {
        g->cellules[x][y] = 0;
    }
}

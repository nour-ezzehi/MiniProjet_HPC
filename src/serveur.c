#include "serveur.h"
#include <stdlib.h>
#include <stdio.h>

void initialiser_serveur(Serveur *s, int nb_robots) {
    s->nb_robots = nb_robots;
    s->new_positions = (int *)malloc(nb_robots * 2 * sizeof(int));
}

void liberer_serveur(Serveur *s) {
    free(s->new_positions);
}

int detecter_collision(int *positions, int nb_robots, int robot_id) {
    int x = positions[robot_id * 2];
    int y = positions[robot_id * 2 + 1];
    
    for (int i = 0; i < nb_robots; i++) {
        if (i != robot_id) {
            int xi = positions[i * 2];
            int yi = positions[i * 2 + 1];
            if (x == xi && y == yi) {
                return 1;
            }
        }
    }
    return 0;
}

void coordonner_robots(Serveur *s, Robot *robots, Grille *g, int nb_robots) {
    // Calculer les nouvelles positions
    for (int i = 0; i < nb_robots; i++) {
        int new_x, new_y;
        calculer_prochain_deplacement(&robots[i], &new_x, &new_y);
        s->new_positions[i * 2] = new_x;
        s->new_positions[i * 2 + 1] = new_y;
    }
    
    // Vérifier les collisions et déplacer
    for (int i = 0; i < nb_robots; i++) {
        if (!robots[i].actif) continue;
        
        int new_x = s->new_positions[i * 2];
        int new_y = s->new_positions[i * 2 + 1];
        
        if (est_cellule_libre(g, new_x, new_y, robots, nb_robots, i)) {
            if (!detecter_collision(s->new_positions, nb_robots, i)) {
                robots[i].x = new_x;
                robots[i].y = new_y;
                
                if (robot_atteint_cible(&robots[i])) {
                    robots[i].actif = 0;
                }
            }
        }
    }
}

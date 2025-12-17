#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include "robot.h"
#include "grille.h"
#include "serveur.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    Config cfg;
    charger_config("data/config.txt", &cfg);
    
    int num_threads = 4;
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }
    
    omp_set_num_threads(num_threads);
    
    printf("=== Simulation OpenMP ===\n");
    printf("Grille: %dx%d, Robots: %d, Iterations: %d, Threads: %d\n\n",
           cfg.taille_grille, cfg.taille_grille, cfg.nb_robots, cfg.nb_iterations, num_threads);
    
    Grille grille;
    initialiser_grille(&grille, cfg.taille_grille);
    
    Robot *robots = (Robot *)malloc(cfg.nb_robots * sizeof(Robot));
    
    for (int i = 0; i < cfg.nb_robots; i++) {
        int x = generer_position_aleatoire(cfg.taille_grille);
        int y = generer_position_aleatoire(cfg.taille_grille);
        int cx = generer_position_aleatoire(cfg.taille_grille);
        int cy = generer_position_aleatoire(cfg.taille_grille);
        initialiser_robot(&robots[i], i, x, y, cx, cy);
        afficher_robot(&robots[i]);
    }
    
    int *new_positions = (int *)malloc(cfg.nb_robots * 2 * sizeof(int));
    
    double debut = omp_get_wtime();
    
    for (int iter = 0; iter < cfg.nb_iterations; iter++) {
        printf("\n--- Iteration %d ---\n", iter + 1);
        
        // Calcul parallèle des nouvelles positions
        #pragma omp parallel for
        for (int i = 0; i < cfg.nb_robots; i++) {
            int new_x, new_y;
            calculer_prochain_deplacement(&robots[i], &new_x, &new_y);
            new_positions[i * 2] = new_x;
            new_positions[i * 2 + 1] = new_y;
        }
        
        // Mise à jour séquentielle pour éviter les conflits
        #pragma omp critical
        {
            for (int i = 0; i < cfg.nb_robots; i++) {
                if (!robots[i].actif) continue;
                
                int new_x = new_positions[i * 2];
                int new_y = new_positions[i * 2 + 1];
                
                if (est_cellule_libre(&grille, new_x, new_y, robots, cfg.nb_robots, i)) {
                    robots[i].x = new_x;
                    robots[i].y = new_y;
                    
                    if (robot_atteint_cible(&robots[i])) {
                        robots[i].actif = 0;
                    }
                }
            }
        }
        
        afficher_grille(&grille, robots, cfg.nb_robots);
        
        int tous_arrives = 1;
        for (int i = 0; i < cfg.nb_robots; i++) {
            if (robots[i].actif) {
                tous_arrives = 0;
                break;
            }
        }
        if (tous_arrives) {
            printf("Tous les robots ont atteint leur cible!\n");
            break;
        }
    }
    
    double fin = omp_get_wtime();
    printf("\nTemps d'exécution: %.6f secondes\n", fin - debut);
    
    free(new_positions);
    liberer_grille(&grille);
    free(robots);
    
    return 0;
}

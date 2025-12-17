#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "robot.h"
#include "grille.h"
#include "serveur.h"
#include "utils.h"

int main(int argc, char *argv[]) {
    srand(time(NULL));
    
    Config cfg;
    charger_config("data/config.txt", &cfg);
    
    printf("=== Simulation Séquentielle ===\n");
    printf("Grille: %dx%d, Robots: %d, Iterations: %d\n\n",
           cfg.taille_grille, cfg.taille_grille, cfg.nb_robots, cfg.nb_iterations);
    
    Grille grille;
    initialiser_grille(&grille, cfg.taille_grille);
    
    Robot *robots = (Robot *)malloc(cfg.nb_robots * sizeof(Robot));
    
    // Initialiser les robots avec positions et cibles aléatoires
    for (int i = 0; i < cfg.nb_robots; i++) {
        int x = generer_position_aleatoire(cfg.taille_grille);
        int y = generer_position_aleatoire(cfg.taille_grille);
        int cx = generer_position_aleatoire(cfg.taille_grille);
        int cy = generer_position_aleatoire(cfg.taille_grille);
        initialiser_robot(&robots[i], i, x, y, cx, cy);
        afficher_robot(&robots[i]);
    }
    
    Serveur serveur;
    initialiser_serveur(&serveur, cfg.nb_robots);
    
    double debut = get_time();
    
    // Affichage initial
    printf("\n--- Iteration 0 (Initial) ---\n");
    afficher_grille(&grille, robots, cfg.nb_robots);

    // Boucle de simulation
    for (int iter = 0; iter < cfg.nb_iterations; iter++) {
        printf("\n--- Iteration %d ---\n", iter + 1);

        coordonner_robots(&serveur, robots, &grille, cfg.nb_robots);

        afficher_grille(&grille, robots, cfg.nb_robots);

        // Vérifier si tous les robots ont atteint leur cible
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

    double fin = get_time();
    printf("\nTemps d'exécution: %.6f secondes\n", fin - debut);

    liberer_serveur(&serveur);
    liberer_grille(&grille);
    free(robots);

    return 0;
}

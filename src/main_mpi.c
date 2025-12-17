#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#include "robot.h"
#include "grille.h"
#include "serveur.h"
#include "utils.h"

#define MASTER 0

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    Config cfg;
    Grille grille;

    Robot *robots = NULL;                 // seulement MASTER
    int *positions = NULL;                // x,y pour chaque robot
    int *nouvelles_positions = NULL;      // résultats Gather
    int *cibles = NULL;                   // cible_x, cible_y, actif

    int my_move[2];                       // déplacement local

    /* ================== INITIALISATION ================== */

    if (rank == MASTER) {
        srand(time(NULL));

        charger_config("data/config.txt", &cfg);

        printf("=== Simulation MPI ===\n");
        printf("Grille: %dx%d | Robots: %d | Iterations: %d | Processus: %d\n\n",
               cfg.taille_grille, cfg.taille_grille,
               cfg.nb_robots, cfg.nb_iterations, size);

        if (size < cfg.nb_robots + 1) {
            fprintf(stderr, "Erreur: besoin d'au moins %d processus MPI\n", cfg.nb_robots + 1);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        initialiser_grille(&grille, cfg.taille_grille);

        robots = malloc(cfg.nb_robots * sizeof(Robot));
        positions = malloc(cfg.nb_robots * 2 * sizeof(int));
        nouvelles_positions = malloc(cfg.nb_robots * 2 * sizeof(int));
        cibles = malloc(cfg.nb_robots * 3 * sizeof(int));

        for (int i = 0; i < cfg.nb_robots; i++) {
            int x = generer_position_aleatoire(cfg.taille_grille);
            int y = generer_position_aleatoire(cfg.taille_grille);
            int cx = generer_position_aleatoire(cfg.taille_grille);
            int cy = generer_position_aleatoire(cfg.taille_grille);

            initialiser_robot(&robots[i], i, x, y, cx, cy);
            afficher_robot(&robots[i]);
        }
    }

    /* Broadcast config */
    MPI_Bcast(&cfg, sizeof(Config), MPI_BYTE, MASTER, MPI_COMM_WORLD);

    /* Workers allocations */
    if (rank != MASTER) {
        positions = malloc(cfg.nb_robots * 2 * sizeof(int));
        cibles = malloc(cfg.nb_robots * 3 * sizeof(int));
    }

    if (rank == MASTER) {
        printf("\n--- Iteration 0 (Initiale) ---\n");
        afficher_grille(&grille, robots, cfg.nb_robots);
    }

    double t0 = MPI_Wtime();

    /* ================== BOUCLE DE SIMULATION ================== */

    for (int iter = 0; iter < cfg.nb_iterations; iter++) {

        if (rank == MASTER) {
            printf("\n--- Iteration %d ---\n", iter + 1);

            /* Préparer positions et cibles */
            for (int i = 0; i < cfg.nb_robots; i++) {
                positions[2*i]   = robots[i].x;
                positions[2*i+1] = robots[i].y;

                cibles[3*i]   = robots[i].cible_x;
                cibles[3*i+1] = robots[i].cible_y;
                cibles[3*i+2] = robots[i].actif;
            }
        }

        /* === 1. Broadcast positions === */
        MPI_Bcast(positions, cfg.nb_robots * 2, MPI_INT, MASTER, MPI_COMM_WORLD);

        /* === 2. Broadcast cibles === */
        MPI_Bcast(cibles, cfg.nb_robots * 3, MPI_INT, MASTER, MPI_COMM_WORLD);

        /* === 3. Calcul local (WORKERS SEULEMENT) === */
        if (rank > 0 && rank <= cfg.nb_robots) {
            int id = rank - 1;

            Robot r;
            r.x = positions[2*id];
            r.y = positions[2*id+1];
            r.cible_x = cibles[3*id];
            r.cible_y = cibles[3*id+1];
            r.actif   = cibles[3*id+2];

            if (r.actif) {
                calculer_prochain_deplacement(&r, &my_move[0], &my_move[1]);
            } else {
                my_move[0] = r.x;
                my_move[1] = r.y;
            }
        }

        /* === 4. Gather vers le robot-chef === */
        int sendbuf[2] = { -1, -1 };

        if (rank > 0 && rank <= cfg.nb_robots) {
            sendbuf[0] = my_move[0];
            sendbuf[1] = my_move[1];
        }

        MPI_Gather(sendbuf, 2, MPI_INT,
                   nouvelles_positions, 2, MPI_INT,
                   MASTER, MPI_COMM_WORLD);

        /* === 5. Validation et application (MASTER) === */
        if (rank == MASTER) {
            for (int i = 0; i < cfg.nb_robots; i++) {
                if (!robots[i].actif) continue;

                int nx = nouvelles_positions[2*i];
                int ny = nouvelles_positions[2*i+1];

                if (est_cellule_libre(&grille, nx, ny, robots, cfg.nb_robots, i)) {
                    robots[i].x = nx;
                    robots[i].y = ny;

                    if (robot_atteint_cible(&robots[i])) {
                        robots[i].actif = 0;
                    }
                }
            }

            afficher_grille(&grille, robots, cfg.nb_robots);
        }
    }

    double t1 = MPI_Wtime();

    /* ================== NETTOYAGE ================== */

    if (rank == MASTER) {
        printf("\nTemps d'execution MPI : %.6f secondes\n", t1 - t0);

        free(robots);
        free(nouvelles_positions);
        liberer_grille(&grille);
    }

    free(positions);
    free(cibles);

    MPI_Finalize();
    return 0;
}

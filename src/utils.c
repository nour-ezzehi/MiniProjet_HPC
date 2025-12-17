#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void charger_config(const char *fichier, Config *cfg) {
    FILE *f = fopen(fichier, "r");
    if (!f) {
        printf("Erreur: Impossible d'ouvrir %s\n", fichier);
        cfg->taille_grille = 20;
        cfg->nb_robots = 5;
        cfg->nb_iterations = 50;
        strcpy(cfg->mode, "SEQUENTIEL");
        return;
    }
    
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), f)) {
        if (strncmp(ligne, "N=", 2) == 0) {
            sscanf(ligne, "N=%d", &cfg->taille_grille);
        } else if (strncmp(ligne, "NB_ROBOTS=", 10) == 0) {
            sscanf(ligne, "NB_ROBOTS=%d", &cfg->nb_robots);
        } else if (strncmp(ligne, "NB_ITERATIONS=", 14) == 0) {
            sscanf(ligne, "NB_ITERATIONS=%d", &cfg->nb_iterations);
        } else if (strncmp(ligne, "MODE=", 5) == 0) {
            sscanf(ligne, "MODE=%s", cfg->mode);
        }
    }
    fclose(f);
}

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

int generer_position_aleatoire(int max) {
    return rand() % max;
}

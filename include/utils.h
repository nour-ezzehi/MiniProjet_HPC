#ifndef UTILS_H
#define UTILS_H

typedef struct {
    int taille_grille;
    int nb_robots;
    int nb_iterations;
    char mode[20];
} Config;

void charger_config(const char *fichier, Config *cfg);
double get_time();
int generer_position_aleatoire(int max);

#endif

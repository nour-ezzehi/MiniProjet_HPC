#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <omp.h>
#include "rle.h"

int main(int argc, char *argv[]) {
    int nb_threads = 4;  // Valeur par défaut
    if (argc > 1) {
        nb_threads = atoi(argv[1]);
    }
    omp_set_num_threads(nb_threads);

    int taille_image = 1024; // Exemple : 1024 octets
    uint8_t *image = malloc(taille_image);

    // Remplissage d'exemple de l'image
#pragma omp parallel for
    for (int i = 0; i < taille_image; i++) {
        image[i] = i % 256;
    }

    int taille_compressee;
    uint8_t *comp = compresser_RLE_openmp(image, taille_image, &taille_compressee);

    printf("Taille originale : %d, taille compressée : %d\n", taille_image, taille_compressee);

    free(image);
    free(comp);

    return 0;
}

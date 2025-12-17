#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mpi.h>
#include "rle.h"

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    uint8_t *image = NULL;
    int taille_image = 1024; // Exemple : 1024 octets

    // Allocate image buffer on all ranks
    image = malloc(taille_image);
    if (!image) {
        MPI_Finalize();
        return 1;
    }

    if (rank == 0) {
        // Remplissage d'exemple de l'image
        for (int i = 0; i < taille_image; i++) {
            image[i] = i % 256;
        }
    }

    // Broadcast image to all ranks
    MPI_Bcast(image, taille_image, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    int taille_compressee = 0;
    uint8_t *comp = compresser_RLE_mpi(image, taille_image, &taille_compressee, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Taille originale : %d, taille compressée : %d\n", taille_image, taille_compressee);
        if (comp) {
            FILE *f = fopen("data/image_rle_mpi.rle", "wb");
            if (f) {
                fwrite(comp, 1, taille_compressee, f);
                fclose(f);
            }
            free(comp);
        }
    }

    free(image);
    MPI_Finalize();
    return 0;
}

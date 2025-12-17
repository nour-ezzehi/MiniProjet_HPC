#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "rle.h"

int main(int argc, char *argv[]) {
    const char *output_file = "data/image.rle";   // fichier compressé

    // Create example image in memory
    int taille_image = 1024;
    uint8_t *image = malloc(taille_image);
    if (!image) return 1;
    for (int i = 0; i < taille_image; i++) image[i] = i % 256;

    // Compression
    int taille_compressee = 0;
    uint8_t *comp = compresser_RLE(image, taille_image, &taille_compressee);
    if (!comp) {
        fprintf(stderr, "Erreur lors de la compression\n");
        free(image);
        return 1;
    }

    // Write compressed to file
    FILE *f = fopen(output_file, "wb");
    if (f) {
        fwrite(comp, 1, taille_compressee, f);
        fclose(f);
    }

    printf("Compression RLE terminée : taille originale = %d -> taille compressée = %d\n", taille_image, taille_compressee);

    // Décompression
    int taille_decomp = 0;
    uint8_t *decomp = decompresser_RLE(comp, taille_compressee, &taille_decomp);
    if (!decomp) {
        fprintf(stderr, "Erreur lors de la décompression\n");
        free(image);
        free(comp);
        return 1;
    }

    const char *decomp_file = "data/image_decomp.bin";
    FILE *fd = fopen(decomp_file, "wb");
    if (fd) {
        fwrite(decomp, 1, taille_decomp, fd);
        fclose(fd);
    }

    printf("Décompression RLE terminée : taille décompressée = %d\n", taille_decomp);

    free(image);
    free(comp);
    free(decomp);

    return 0;
}

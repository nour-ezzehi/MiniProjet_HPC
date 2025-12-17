#include <stdlib.h>
#include "rle.h"

// Compression séquentielle
uint8_t* compresser_RLE(const uint8_t *img, int taille, int *out_size) {
    if (!img || taille <= 0) return NULL;

    uint8_t *comp = malloc(taille * 2); // max possible
    int index = 0;

    for (int i = 0; i < taille;) {
        uint8_t val = img[i];
        int count = 1;
        while (i + count < taille && img[i+count] == val && count < 255) {
            count++;
        }
        comp[index++] = val;
        comp[index++] = count;
        i += count;
    }

    *out_size = index;
    return comp;
}

// Décompression séquentielle
uint8_t* decompresser_RLE(const uint8_t *data, int taille, int *out_size) {
    if (!data || taille <= 0) return NULL;

    // Estimer taille maximale
    int max_size = 0;
    for (int i = 1; i < taille; i += 2) max_size += data[i];

    uint8_t *img = malloc(max_size);
    int idx = 0;

    for (int i = 0; i < taille; i += 2) {
        uint8_t val = data[i];
        int count = data[i+1];
        for (int j = 0; j < count; j++) img[idx++] = val;
    }

    *out_size = idx;
    return img;
}

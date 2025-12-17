#include <stdlib.h>
#include <omp.h>
#include "rle.h"

uint8_t* compresser_RLE_openmp(const uint8_t *img, int taille, int *out_size) {
    return compresser_RLE(img, taille, out_size);
}

uint8_t* decompresser_RLE_openmp(const uint8_t *data, int taille, int *out_size) {
    return decompresser_RLE(data, taille, out_size);
}

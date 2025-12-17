#ifndef RLE_H
#define RLE_H

#include <stdint.h>

uint8_t* compresser_RLE(const uint8_t *img, int taille, int *out_size);
uint8_t* decompresser_RLE(const uint8_t *data, int taille, int *out_size);

uint8_t* compresser_RLE_openmp(const uint8_t *img, int taille, int *out_size);
uint8_t* decompresser_RLE_openmp(const uint8_t *data, int taille, int *out_size);

/* Declare MPI-aware prototype if MPI headers are available; otherwise declare with a generic type for compatibility. */
#if defined(__has_include)
  #if __has_include(<mpi.h>)
    #include <mpi.h>
    uint8_t* compresser_RLE_mpi(const uint8_t *img, int taille, int *out_size, MPI_Comm comm);
  #else
    typedef void* MPI_Comm; /* fallback type when mpi.h isn't present during non-MPI builds */
    uint8_t* compresser_RLE_mpi(const uint8_t *img, int taille, int *out_size, MPI_Comm comm);
  #endif
#else
  /* Conservative fallback: declare MPI_Comm as void* */
  typedef void* MPI_Comm;
  uint8_t* compresser_RLE_mpi(const uint8_t *img, int taille, int *out_size, MPI_Comm comm);
#endif

#endif

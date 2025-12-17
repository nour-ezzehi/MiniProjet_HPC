#if defined(__has_include)
  #if __has_include(<mpi.h>)
    #include <mpi.h>
    #define HAVE_MPI 1
  #else
    #define HAVE_MPI 0
  #endif
#else
  /* Conservative default: assume no MPI available */
  #define HAVE_MPI 0
#endif

#include <stdlib.h>
#include "rle.h"

#if HAVE_MPI

// Chaque processus reçoit une portion de l'image
// Master : distribue blocs d'image et rassemble résultats
uint8_t* compresser_RLE_mpi(const uint8_t *img, int taille, int *out_size, MPI_Comm comm) {
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int block = taille / size;
    int start = rank * block;
    int end = (rank == size-1) ? taille : start + block;

    int local_size;
    uint8_t *local_comp = compresser_RLE(img + start, end - start, &local_size);

    int *sizes = NULL;
    if (rank == 0) sizes = malloc(size * sizeof(int));
    MPI_Gather(&local_size, 1, MPI_INT, sizes, 1, MPI_INT, 0, comm);

    uint8_t *result = NULL;
    int *displs = NULL;
    if (rank == 0) {
        displs = malloc(size * sizeof(int));
        int total = 0;
        for (int i = 0; i < size; i++) {
            displs[i] = total;
            total += sizes[i];
        }
        result = malloc(total);
    }

    MPI_Gatherv(local_comp, local_size, MPI_UNSIGNED_CHAR,
                result, sizes, displs, MPI_UNSIGNED_CHAR,
                0, comm);

    free(local_comp);
    if (rank == 0) {
        *out_size = displs[size-1] + sizes[size-1];
        free(sizes);
        free(displs);
    }

    return result;
}

#else

// Fallback when MPI is not available: single-process wrapper
typedef void* MPI_Comm; /* keep signature compatible */
uint8_t* compresser_RLE_mpi(const uint8_t *img, int taille, int *out_size, MPI_Comm comm) {
    (void)comm; // unused
    return compresser_RLE(img, taille, out_size);
}

#endif

CC = gcc
MPICC = mpicc
CFLAGS = -Wall -Iinclude
OMPFLAGS = -fopenmp
LDFLAGS = -lm

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Fichiers sources communs
COMMON_SRCS = $(SRC_DIR)/robot.c $(SRC_DIR)/grille.c $(SRC_DIR)/serveur.c $(SRC_DIR)/utils.c
COMMON_OBJS = $(OBJ_DIR)/robot.o $(OBJ_DIR)/grille.o $(OBJ_DIR)/serveur.o $(OBJ_DIR)/utils.o

# Fichiers sources RLE
RLE_SRCS = $(SRC_DIR)/rle_sequentiel.c $(SRC_DIR)/rle_openmp.c $(SRC_DIR)/rle_mpi.c
RLE_OBJS = $(OBJ_DIR)/rle_sequentiel.o $(OBJ_DIR)/rle_openmp.o $(OBJ_DIR)/rle_mpi.o

# Exécutables simulation
SIM_TARGETS = $(BIN_DIR)/simulation_seq $(BIN_DIR)/simulation_omp $(BIN_DIR)/simulation_mpi
# Exécutables RLE
RLE_TARGETS = $(BIN_DIR)/rle_seq $(BIN_DIR)/rle_omp $(BIN_DIR)/rle_mpi

all: dirs $(SIM_TARGETS) $(RLE_TARGETS)

dirs:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR) data

# Explicit rule to compile MPI source with mpicc (needs mpi.h)
$(OBJ_DIR)/rle_mpi.o: $(SRC_DIR)/rle_mpi.c
	$(MPICC) $(CFLAGS) -c $< -o $@

# ---------------- Compilation des objets ----------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# ---------------- Simulation ----------------
$(BIN_DIR)/simulation_seq: $(SRC_DIR)/main_sequentiel.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/simulation_omp: $(SRC_DIR)/main_openmp.c $(COMMON_OBJS)
	$(CC) $(CFLAGS) $(OMPFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/simulation_mpi: $(SRC_DIR)/main_mpi.c $(COMMON_OBJS)
	$(MPICC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ---------------- RLE ----------------
$(BIN_DIR)/rle_seq: $(SRC_DIR)/main_rle_sequentiel.c $(OBJ_DIR)/rle_sequentiel.o
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/rle_omp: $(SRC_DIR)/main_rle_openmp.c $(OBJ_DIR)/rle_openmp.o $(OBJ_DIR)/rle_sequentiel.o
	$(CC) $(CFLAGS) $(OMPFLAGS) $^ -o $@ $(LDFLAGS)

$(BIN_DIR)/rle_mpi: $(SRC_DIR)/main_rle_mpi.c $(OBJ_DIR)/rle_mpi.o $(OBJ_DIR)/rle_sequentiel.o
	$(MPICC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ---------------- Exécution ----------------
run_seq: $(BIN_DIR)/simulation_seq
	./$(BIN_DIR)/simulation_seq

run_omp: $(BIN_DIR)/simulation_omp
	./$(BIN_DIR)/simulation_omp 4

run_mpi: $(BIN_DIR)/simulation_mpi
	mpirun --oversubscribe -np 5 ./$(BIN_DIR)/simulation_mpi

run_rle_seq: $(BIN_DIR)/rle_seq
	./$(BIN_DIR)/rle_seq

run_rle_omp: $(BIN_DIR)/rle_omp
	./$(BIN_DIR)/rle_omp 4

run_rle_mpi: $(BIN_DIR)/rle_mpi
	mpirun --oversubscribe -np 5 ./$(BIN_DIR)/rle_mpi

# ---------------- Benchmark ----------------
benchmark:
	@echo "=== Benchmark Sequential ==="
	@./$(BIN_DIR)/simulation_seq | grep "Temps"
	@echo "\n=== Benchmark OpenMP (2 threads) ==="
	@./$(BIN_DIR)/simulation_omp 2 | grep "Temps"
	@echo "\n=== Benchmark OpenMP (4 threads) ==="
	@./$(BIN_DIR)/simulation_omp 4 | grep "Temps"
	@echo "\n=== Benchmark OpenMP (8 threads) ==="
	@./$(BIN_DIR)/simulation_omp 8 | grep "Temps"
	@echo "\n=== Benchmark MPI (3 processus) ==="
	@mpirun -np 5 ./$(BIN_DIR)/simulation_mpi | grep "Temps"
	@echo "\n=== Benchmark MPI (4 processus) ==="
	@mpirun -np 5 ./$(BIN_DIR)/simulation_mpi | grep "Temps"

# ---------------- Nettoyage ----------------
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all dirs clean run_seq run_omp run_mpi run_rle_seq run_rle_omp run_rle_mpi benchmark

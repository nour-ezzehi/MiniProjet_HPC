# Simulation de Robots Mobiles avec HPC

Ce projet implémente une simulation de robots mobiles se déplaçant sur une grille, ainsi qu'un module de compression d'images par Run-Length Encoding (RLE). Trois approches de la simulation sont fournies : séquentielle, OpenMP (mémoire partagée) et MPI (mémoire distribuée).

## Table des matières

- Présentation
- Structure du projet
- Fonctionnalités principales
  - Simulation (séquentielle / OpenMP / MPI)
  - Compression RLE (séquentielle / OpenMP / MPI)
- Compilation
- Exécution et exemples
  - Simulation
  - RLE (compression / décompression)
- Format RLE et détails techniques
- Configuration
- Tests et benchmarks
- Dépannage
- Licence

---

## Présentation

Objectifs pédagogiques :

- Mettre en œuvre et comparer différentes formes de parallélisme (OpenMP, MPI).
- Simuler la coordination et l'évitement de collisions d'un ensemble de robots sur une grille 2D.
- Implémenter et expérimenter une compression simple d'images par Run-Length Encoding (RLE).

La partie simulation illustre la coordination maître/esclave (serveur + robots), la synchronisation et la gestion des conflits de déplacement. La partie RLE est un module autonome pour compresser/décompresser des images stockées sous forme textuelle ou binaire simple.

## Structure du projet

```
├── README.md
├── Makefile
├── bin/                # exécutables générés
│   ├── rle_seq
│   ├── rle_omp
│   ├── rle_mpi
│   ├── simulation_seq
│   ├── simulation_omp
│   └── simulation_mpi
├── include/            # en-têtes
├── src/                # sources (simulation, rle, utils)
├── obj/                # objets compilés
└── data/               # exemples d'images / configurations
    ├── config.txt
    ├── image.rle
    └── image_decomp.bin
```

## Fonctionnalités principales

### Simulation

- Grille 2D N×N
- Robots avec position, cible, ID et état
- Serveur/robot-chef qui valide les propositions de déplacements
- Implémentations : séquentielle, OpenMP (threads), MPI (processus)
- Affichage console de la grille (optionnel, dépend du build)

### Compression RLE

- Outil pour compresser et décompresser des images (ou données 1D/2D) avec Run-Length Encoding
- Implémentations séquentielle, OpenMP, MPI (pour étude comparée)
- Format simple : paires (valeur, longueur) ou format binaire dépendant de l'implémentation

## Compilation

Construire tout le projet :

```bash
make all
```

Construire un exécutable précis (exemples) :

```bash
make bin/simulation_seq
make bin/simulation_omp
make bin/simulation_mpi
make bin/rle_seq
make bin/rle_omp
make bin/rle_mpi
```

Remarque : les cibles `make` dépendent du `Makefile` fourni ; `make all` est la façon la plus sûre.

Prérequis : GCC (avec OpenMP), MPI (OpenMPI ou MPICH), make.

## Exécution et exemples

### Simulation

Version séquentielle :

```bash
./bin/simulation_seq           # utilise data/config.txt par défaut
```

OpenMP (exemple 4 threads) :

```bash
export OMP_NUM_THREADS=4
./bin/simulation_omp
# ou ./bin/simulation_omp 4 si le binaire accepte un argument
```

MPI (exemple 6 processus) :

```bash
mpirun -np 6 ./bin/simulation_mpi
```

### RLE — compression et décompression

Les exécutables RLE compressent ou décompressent des fichiers. Exemple d'utilisation (format CLI générique) :

```bash
# Compression (séquentielle)
./bin/rle_seq data/image_decomp.bin data/image.rle

# Décompression
./bin/rle_seq -d data/image.rle data/image_decomp.bin

# OpenMP version
export OMP_NUM_THREADS=4
./bin/rle_omp data/image_decomp.bin data/image_rle_omp.rle

# MPI version (exécution distribuée)
mpirun -np 4 ./bin/rle_mpi data/image_decomp.bin data/image_rle_mpi.rle
```

Les noms d'options peuvent varier légèrement selon la version compilée ; essayez `./bin/rle_seq --help` ou `-h` pour obtenir la syntaxe exacte.

Fichiers d'exemple fournis dans `data/` :

- `data/image_decomp.bin` : image non compressée (octets ou texte selon l'exemple)
- `data/image.rle` : image compressée (exemple)

## Format RLE et détails techniques

Le Run-Length Encoding (RLE) est une compression sans perte très simple :

- On parcourt les données ligneairement.
- Pour chaque valeur consécutive identique, on enregistre la valeur et le nombre de répétitions.

Détails d'implémentation :

- Les versions séquentielles, OpenMP et MPI de la compression RLE sont fournies pour comparaison.
- La décompression RLE est également implémentée pour chaque version.

## Configuration

Modifiez le fichier `data/config.txt` pour ajuster les paramètres :

```
N=20                # Taille de la grille (20x20)
NB_ROBOTS=5         # Nombre de robots
NB_ITERATIONS=50    # Nombre maximum d'itérations
MODE=OPENMP         # Mode (information seulement)
```

## Tests et benchmarks

Pour comparer les performances des différentes versions :

```bash
make benchmark
```

Cela exécutera toutes les versions et affichera les temps d'exécution.

Des tests spécifiques à la RLE peuvent être ajoutés dans `tests/` si nécessaire.

## Dépannage

Erreurs courantes :

- **Problèmes de compilation** : Assurez-vous d'avoir installé toutes les dépendances (GCC, MPI, make).
- **Erreurs d'exécution** : Vérifiez les fichiers de configuration et les chemins d'accès aux données.
- **Problèmes de performance** : Consultez les sections sur la configuration et l'exécution pour vous assurer que vous utilisez les bons paramètres.

## Licence

Ce projet est sous licence MIT. Voir le fichier `LICENSE` pour plus de détails.

---

Fin du README.

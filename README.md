# K-Means Clustering in C

A from-scratch implementation of the **K-Means Clustering** algorithm written in C.
The program reads 2-D data points from a text file, groups them into *k* clusters, and writes each point's cluster assignment to an output file.

---

## Table of Contents

- [Features](#features)
- [Project Structure](#project-structure)
- [Prerequisites](#prerequisites)
- [Build & Run](#build--run)
- [Interactive Walkthrough](#interactive-walkthrough)
- [Algorithm Overview](#algorithm-overview)
- [Key Functions](#key-functions)
- [Datasets](#datasets)
- [Debugging & Memory Checks](#debugging--memory-checks)

---

## Features

| Feature | Details |
|---|---|
| Centroid initialization | Random selection from data **or** manual entry |
| Distance metric | Euclidean distance |
| Convergence | Component-wise tolerance (`0.0001`) with a 100-iteration cap |
| Input datasets | Small (1 798 pts) and large (10 000 pts) bundled |
| Output | Tab-separated coordinates + 1-based cluster index |
| Memory safety | Zero leaks confirmed with Valgrind |
| Error handling | Invalid files, bad input values, duplicate centroids |

---

## Project Structure

```
k-mean-clustering-in-C/
├── main.c            # Entry point — user prompts & orchestration
├── kmeans.c          # Algorithm core — all clustering logic
├── Makefile          # Build, clean, valgrind, and debug targets
├── kmeans-data.txt   # Small dataset  (1 798 two-dimensional points)
├── large-data.txt    # Large dataset  (10 000 two-dimensional points)
└── README.md
```

---

## Prerequisites

| Tool | Purpose |
|---|---|
| GCC | Compilation |
| GNU Make | Build automation |
| Valgrind *(optional)* | Memory-leak detection |
| GDB *(optional)* | Interactive debugger |


---

## Build & Run

```bash
# 1. Clone the repository

# 2. Compile
make

# 3. Execute
./kmeans

# 4. Remove build artifacts
make clean
```

---

## Interactive Walkthrough

When you run `./kmeans` the program guides you through three prompts:

```
Select the dataset to use:
  1. kmeans-data.txt
  2. large-data.txt
  3. Exit
Enter your choice (1, 2 or 3): 1

Provide number of clusters (must be >= 1 and <= number of data points): 4

Choose centroid initialization method:
  1. Randomly select centroids
  2. Enter centroids manually
  3. Exit
Enter your choice (1, 2, or 3): 1

Clustered data with assignments written to kmeans-output.txt
```

The output file `kmeans-output.txt` contains one line per data point:

```
x_value  y_value   cluster_id
-1.26    21.27     3
7.96    -20.77     2
...
```

---

## Algorithm Overview

```
Initialize k centroids
        │
        ▼
┌─────────────────────────────┐
│  Assign each point to the   │
│  nearest centroid           │  ◄─────────┐
└──────────────┬──────────────┘            │
               │                           │
               ▼                           │
┌─────────────────────────────┐            │
│  Recompute each centroid    │            │
│  as the mean of its cluster │            │
└──────────────┬──────────────┘            │
               │                           │
               ▼                           │
      Centroids changed                    │
      beyond tolerance? ──── Yes ──────────┘
               │
              No
               │
               ▼
        Write output file
```

Convergence is checked component-wise:
`|new_centroid[i][j] − old_centroid[i][j]| ≤ 0.0001` for all *i*, *j*.

---

## Datasets

| File | Points | Dimensions | Notes |
|---|---|---|---|
| `kmeans-data.txt` | 1 798 | 2 | Default test set |
| `large-data.txt` | 10 000 | 2 | Stress/performance test |

Each line contains two tab-separated floating-point values representing *(x, y)* coordinates.

---

## Debugging & Memory Checks

```bash
# Run under Valgrind (checks for leaks; reports wall-clock time)
make valgrind

# Open in GDB for interactive debugging
make debug
```

Valgrind target requires the `kmeans` binary to be built first (`make` handles this automatically).

# K-Means Clustering Algorithm in C

## Overview
This project implements the **K-Means Clustering Algorithm** in C. The project involves clustering 2-dimensional data points using user-specified cluster counts and generating an output file with the clustering results. The implementation demonstrates skills in dynamic memory allocation, pointer manipulation, file I/O, and modular code design in a Unix environment.

## Features
- **Dynamic Memory Allocation**: Efficiently handles memory for large datasets.
- **K-Means Algorithm**: Clusters data points into user-specified clusters.
- **File I/O**: Reads data from a text file and outputs results to another text file.
- **Makefile Integration**: Automates compilation, linking, and memory checks.
- **Memory Management**: Ensures no memory leaks using tools like Valgrind.
- **Edge Case Handling**: Robustly handles scenarios like `k=1` or `k=number of data points`.
- **Large Dataset Support**: Successfully processes datasets with up to 10,000 data points.

## Files in the Repository
```
project_directory/
├── Makefile                # Automates compilation, linking, and debugging
├── README.md               # Documentation for the project
├── kmeans                  # Executable binary file
├── kmeans-data.txt         # Input data file 
├── kmeans-output.txt       # Output file with clustering results
├── kmeans.c                # Source file containing K-means algorithm implementation
├── kmeans.o                # Object file for kmeans.c
├── large-data.txt          # Additional large dataset file for testing (10,000 data points included)
├── main.c                  # Source file containing main function
└── main.o                  # Object file for main.c
```

## How to Run the Project
### Prerequisites
- **Operating System**: Unix-based (Ubuntu 24.04 or Fedora 40 recommended).
- **Compiler**: GCC.
- **Utilities**: `make`, `valgrind` (for memory leak detection), `gdb` (for debugging).

### Steps
1. Clone the repository:
   ```bash
   git clone <repository_url>
   cd project_directory
   ```
2. Compile the program using the Makefile:
   ```bash
   make
   ```
3. Run the executable:
   ```bash
   ./kmeans
   ```
4. The program will:
   - Prompt the user to choose a dataset (`kmeans-data.txt` or `large-data.txt`).
   - Ask the user for the number of clusters.
   - Ask the user whether to manually enter centroids or use randomly selected centroids.
   - Perform clustering using the K-means algorithm.
   - Generate an output file, `kmeans-output.txt`, containing the original data and cluster assignments.
5. To clean up object files and binaries:
   ```bash
   make clean
   ```

### Debugging and Memory Leak Detection
- To run Valgrind for memory leak checks:
  ```bash
  make valgrind
  ```
- To debug using GDB:
  ```bash
  make gdb
  ```

## Dataset
- **Input**: `kmeans-data.txt` with 1,798 data points.
- **Input**: `large-data.txt ` with 10,000 data points.
- **Output**: `kmeans-output.txt` with cluster assignments.

## K-Means Algorithm
### Steps
1. Initialize centroids by selecting `k` random points or using user-provided centroids.
2. Assign each point to the nearest centroid.
3. Update centroids based on the mean of points in each cluster.
4. Repeat steps 2-3 until convergence.

### Key Functions
- **Centroid Initialization**: Random or user-specified.
- **Distance Calculation**: Euclidean distance.
- **Cluster Assignment**: Nearest centroid.
- **Centroid Update**: Mean of assigned points.

## Project Highlights
- Successfully clustered a dataset with 10,000 points.
- Implemented robust error handling for missing/invalid files and inputs.
- Optimized for performance with modular code design.

## Execution Overview
This project was executed as follows:
- Developed modular code to handle data reading, clustering logic, and output writing.
- Performed correctly for a dataset of 10,000 data points in `large-data.txt`.
- Implemented robust error handling for missing files and invalid inputs.
- Integrated debugging and memory management tools (Valgrind and GDB).
- Designed a Makefile to automate compilation and testing.

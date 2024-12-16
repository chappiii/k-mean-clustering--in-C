#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>
#define maxIterations 100


// Function to read the matrix from a file
double** readMatrixFromFile(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    // Check if the file is empty
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size == 0) {
        fprintf(stderr, "Error: File is empty.\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    rewind(file);

    *rows = 0;
    *cols = 0;
    char ch;
    int temp_cols = 0;
    int countingCols = 1;

    // Count the number of rows and columns
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            if (temp_cols > 0) {
                (*rows)++;
                if (countingCols) {
                    (*cols) = temp_cols + 1;
                    countingCols = 0;
                }
            }
            temp_cols = 0;
        } else if (ch == ' ' || ch == '\t') {
            temp_cols++;
        }
    }

    // Handle case where file doesn't end with a newline
    if (temp_cols > 0) {
        (*rows)++;
    }

    rewind(file);

    // Allocate memory for the matrix and initialize to 0
    double** matrix = (double**)calloc(*rows, sizeof(double*));
    for (int i = 0; i < *rows; i++) {
        matrix[i] = (double*)calloc(*cols, sizeof(double));
    }

    // Read Matrix Data from the File
    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            int result = fscanf(file, "%lf", &matrix[i][j]);

            // Handle empty or invalid lines
            if (result != 1) {
                fprintf(stderr, "Error reading value at row %d, col %d. Invalid input or empty line encountered.\n", i, j);
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
    return matrix;
}

// Function to calculate the Euclidean distance between two points
double euclideanDistance(double* point1, double* point2, int cols) {
    double sum = 0.0;
    for (int i = 0; i < cols; i++) {
        sum += (point1[i] - point2[i]) * (point1[i] - point2[i]);
    }
    return sqrt(sum);
}

// Function to initialize centroids randomly from the data
double** selectRandomCentroids(double** data, int rows, int cols, int k) {
    srand(time(0));
    double** centroids = (double**)malloc(k * sizeof(double*));
    int* selected = (int*)calloc(rows, sizeof(int));

    for (int i = 0; i < k; i++) {
        centroids[i] = (double*)malloc(cols * sizeof(double));
        int randomIndex;
        do {
            randomIndex = rand() % rows;
        } while (selected[randomIndex] == 1);

        selected[randomIndex] = 1;
        for (int j = 0; j < cols; j++) {
            centroids[i][j] = data[randomIndex][j];
        }
    }

    free(selected);
    return centroids;
}

// Function to assign points to clusters
void assignClusters(double** data, double** centroids, int* assignments, int rows, int cols, int k, double*** clusters, int* clusterSizes) {
    // Reset cluster sizes to zero at the start of each iteration
    for (int i = 0; i < k; i++) {
        clusterSizes[i] = 0;
    }

    // Assign each point to the nearest centroid
    for (int i = 0; i < rows; i++) {
        double minDistance = euclideanDistance(data[i], centroids[0], cols);  // Initialize with distance to the first centroid
        int closestCentroid = 0;

        // Find the closest centroid
        for (int j = 1; j < k; j++) {
            double distance = euclideanDistance(data[i], centroids[j], cols);
            if (distance < minDistance) {
                minDistance = distance;
                closestCentroid = j;
            }
        }

        // Assign the data point to the closest centroid
        assignments[i] = closestCentroid;

        // Place the data point in the appropriate cluster
        int index = clusterSizes[closestCentroid];
        clusters[closestCentroid][index] = data[i];
        clusterSizes[closestCentroid]++;
    }
}

// Function to calculate the means of clusters
void calculateClusterMeans(double*** clusters, double** centroids, int* clusterSizes, int k, int cols) {
    // Loop through each cluster
    for (int i = 0; i < k; i++) {
        // Reset the centroid values to 0
        for (int j = 0; j < cols; j++) {
            centroids[i][j] = 0.0;
        }

        // If the cluster is empty, skip the mean calculation
        if (clusterSizes[i] == 0) continue;

        // Sum up the points in the cluster
        for (int j = 0; j < clusterSizes[i]; j++) {
            for (int l = 0; l < cols; l++) {
                centroids[i][l] += clusters[i][j][l];
            }
        }

        // Divide by the number of points in the cluster to get the mean
        for (int l = 0; l < cols; l++) {
            centroids[i][l] /= clusterSizes[i];
        }
    }
}

// Function to free the 2D matrix
void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to check convergence
int hasConverged(double** oldCentroids, double** newCentroids, int k, int cols, double tolerance) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < cols; j++) {
            if (fabs(newCentroids[i][j] - oldCentroids[i][j]) > tolerance) {
                return 0;
            }
        }
    }
    return 1;
}

// Function to write clustered data to a file
void writeClusteredFile(const char* filename, double** data, int* assignments, int rows, int cols) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing results");
        exit(EXIT_FAILURE);
    }

    // Write each data point with its cluster assignment
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%lf ", data[i][j]);
        }
        // Print the cluster assignment (add one to make it human-readable, 1-based index)
        fprintf(file, " %d\n", assignments[i] + 1);
    }

    fclose(file);
    printf("Clustered data with assignments written to %s\n", filename);
}

// Function to free the 3D array of clusters
void freeClusters(double*** clusters, int* clusterSizes, int k) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < clusterSizes[i]; j++) {
            free(clusters[i][j]);
        }
        free(clusters[i]);
    }
    free(clusters);
    free(clusterSizes);
}

// Function to compare two centroids to see if they are equal
bool areCentroidsEqual(double* centroid1, double* centroid2, int cols) {
    for (int i = 0; i < cols; i++) {
        if (centroid1[i] != centroid2[i]) {
            return false;  // Centroids are different
        }
    }
    return true;  // Centroids are equal
}

void kmeansClustering(double** data, int rows, int cols, int k, double tolerance, int* assignments, double** centroids) {
    double** oldCentroids = (double**)malloc(k * sizeof(double*));
    for (int i = 0; i < k; i++) {
        oldCentroids[i] = (double*)malloc(cols * sizeof(double));
    }

    // Allocate memory for clusterSizes and clusters
    int* clusterSizes = (int*)calloc(k, sizeof(int));
    double*** clusters = (double***)malloc(k * sizeof(double**));
    for (int i = 0; i < k; i++) {
        clusters[i] = (double**)malloc(rows * sizeof(double*));
    }

    // main loop
    for (int iter = 0; iter < maxIterations; iter++) {
        // Copy centroids to oldCentroids
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < cols; j++) {
                oldCentroids[i][j] = centroids[i][j];
            }
        }

        // Assign clusters
        assignClusters(data, centroids, assignments, rows, cols, k, clusters, clusterSizes);

        // Calculate new centroids
        calculateClusterMeans(clusters, centroids, clusterSizes, k, cols);

        // Check for convergence
        if (hasConverged(oldCentroids, centroids, k, cols, tolerance)) {
            break;
        }
    }

    // Free the memory allocated for clusters and other arrays
    for (int i = 0; i < k; i++) {
        free(clusters[i]);
        free(oldCentroids[i]);
    }
    free(clusters);
    free(clusterSizes);
    free(oldCentroids);
}

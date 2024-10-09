#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> 

// Function to read the matrix from a file
double** readMatrixFromFile(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    *rows = 0;
    *cols = 0;
    char ch;
    int temp_cols = 0;
    int countingCols = 1;

    // Count the number of rows and columns
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            (*rows)++;
            if (countingCols) {
                (*cols) = temp_cols + 1;
                countingCols = 0;
            }
            temp_cols = 0;
        } else if (ch == ' ' || ch == '\t') {
            temp_cols++;
        }
    }

    if (ch != '\n') {
        (*rows)++;
    }

    rewind(file);

    // Allocate memory for the matrix
    double** matrix = (double**)malloc(*rows * sizeof(double*));
    for (int i = 0; i < *rows; i++) {
        matrix[i] = (double*)malloc(*cols * sizeof(double));
    }

    for (int i = 0; i < *rows; i++) {
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%lf", &matrix[i][j]);
        }
    }

    fclose(file);
    return matrix;
}

// Function to calculate the Euclidean distance between two points
double euclideanDistance(double* point, double* centroid, int cols) {
    double sum = 0.0;
    for (int i = 0; i < cols; i++) {
        sum += (point[i] - centroid[i]) * (point[i] - centroid[i]);
    }
    return sqrt(sum);
}

// Function to initialize centroids randomly
double** selectRandomCentroids(double** data, int rows, int cols, int k) {
    srand(time(0));  // Initialize random seed
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
double*** assignClusters(double** data, double** centroids, int* assignments, int rows, int cols, int k, int** clusterSizes) {
    // Initialize cluster sizes to 0
    *clusterSizes = (int*)calloc(k, sizeof(int));

    // Step 1: Assign each point to the nearest centroid
    for (int i = 0; i < rows; i++) {
        double minDistance = euclideanDistance(data[i], centroids[0], cols);  // Initialize with distance to the first centroid
        int closestCentroid = 0;

        // Loop over all centroids to find the closest one
        for (int j = 1; j < k; j++) {
            double distance = euclideanDistance(data[i], centroids[j], cols);
            if (distance < minDistance) {
                minDistance = distance;
                closestCentroid = j;
            }
        }

        // Assign the data point to the closest centroid
        assignments[i] = closestCentroid;

        // Increment the size of the cluster
        (*clusterSizes)[closestCentroid]++;
    }

    // Step 2: Allocate memory for each cluster based on the number of points assigned to it
    double*** clusters = (double***)malloc(k * sizeof(double**));  // Allocate array for each cluster

    // Allocate memory for each cluster based on its size
    for (int i = 0; i < k; i++) {
        clusters[i] = (double**)malloc((*clusterSizes)[i] * sizeof(double*));
        for (int j = 0; j < (*clusterSizes)[i]; j++) {
            clusters[i][j] = (double*)malloc(cols * sizeof(double));  // Each point in the cluster has `cols` features
        }
    }

    // Step 3: Re-assign data points to their respective clusters
    int* clusterIndices = (int*)calloc(k, sizeof(int));  // Track where to place the next point in each cluster

    for (int i = 0; i < rows; i++) {
        int clusterID = assignments[i];
        int index = clusterIndices[clusterID];  // Find the next index to place the data point in this cluster
        for (int j = 0; j < cols; j++) {
            clusters[clusterID][index][j] = data[i][j];  // Copy data point into the cluster
        }
        clusterIndices[clusterID]++;  // Increment the position for the next data point in this cluster
    }

    free(clusterIndices);  // Free temporary memory for tracking indices
    return clusters;  // Return the array of clusters
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
                centroids[i][l] += clusters[i][j][l];  // Add each feature value to the centroid sum
            }
        }

        // Divide by the number of points in the cluster to get the mean
        for (int l = 0; l < cols; l++) {
            centroids[i][l] /= clusterSizes[i];  // Calculate the mean for each feature
        }

        // Optionally, print the mean (new centroid) for each cluster
        printf("New Centroid %d: (", i + 1);
        for (int l = 0; l < cols; l++) {
            printf("%lf", centroids[i][l]);
            if (l < cols - 1) printf(", ");
        }
        printf(")\n");
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
                return 0;  // Centroids have not converged
            }
        }
    }
    return 1;  // Centroids have converged
}

// Function to write clustered data to a file
void writeClusteredData(const char* filename, double** data, int* assignments, int rows, int cols) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file for writing results");
        exit(EXIT_FAILURE);
    }

    // Write the header
    fprintf(file, "Data Point,Cluster Assignment\n");

    // Write each data point with its cluster assignment
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            fprintf(file, "%lf ", data[i][j]);
        }
        // Print the cluster assignment (add one to make it human-readable, 1-based index)
        fprintf(file, "-> Cluster %d\n", assignments[i] + 1);
    }

    fclose(file);
    printf("Clustered data with assignments written to %s\n", filename);
}

// Function to free the 3D array of clusters
void freeClusters(double*** clusters, int* clusterSizes, int k) {
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < clusterSizes[i]; j++) {
            free(clusters[i][j]);  // Free each data point
        }
        free(clusters[i]);  // Free the cluster array
    }
    free(clusters);  // Free the outer array
    free(clusterSizes);  // Free the array that stores the size of each cluster
}

// Core function to run the K-means clustering algorithm
void kmeansClustering(double** data, int rows, int cols, int k, double tolerance, int* assignments, double** centroids) {
    double** oldCentroids = (double**)malloc(k * sizeof(double*));
    for (int i = 0; i < k; i++) {
        oldCentroids[i] = (double*)malloc(cols * sizeof(double));
    }

    int maxIterations = 100;
    int converged = 0;
    for (int iter = 0; iter < maxIterations; iter++) {
        // Copy centroids to oldCentroids
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < cols; j++) {
                oldCentroids[i][j] = centroids[i][j];
            }
        }

        // Assign clusters and calculate new centroids
        int* clusterSizes = NULL;
        double*** clusters = assignClusters(data, centroids, assignments, rows, cols, k, &clusterSizes);
        calculateClusterMeans(clusters, centroids, clusterSizes, k, cols);

        // Check for convergence
        if (hasConverged(oldCentroids, centroids, k, cols, tolerance)) {
            converged = 1;
            freeClusters(clusters, clusterSizes, k);
            break;
        }

        // Free clusters for next iteration
        freeClusters(clusters, clusterSizes, k);
    }

    for (int i = 0; i < k; i++) {
        free(oldCentroids[i]);
    }
    free(oldCentroids);
}

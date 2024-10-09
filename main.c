#include <stdio.h>
#include <stdlib.h>

// Function prototypes
double** readMatrixFromFile(const char* filename, int* rows, int* cols);
double euclideanDistance(double* point, double* centroid, int cols);
double** selectRandomCentroids(double** data, int rows, int cols, int k);
double*** assignClusters(double** data, double** centroids, int* assignments, int rows, int cols, int k, int** clusterSizes);
void calculateClusterMeans(double*** clusters, double** centroids, int* clusterSizes, int k, int cols);
void freeMatrix(double** matrix, int rows);
int hasConverged(double** oldCentroids, double** newCentroids, int k, int cols, double tolerance);
void writeClusteredData(const char* filename, double** data, int* assignments, int rows, int cols);
void kmeansClustering(double** data, int rows, int cols, int k, double tolerance, int* assignments);

int main() {
    int rows, cols, k;
    double tolerance = 0.0001;

    // Read the data from the file using a function from kmeans.c
    double** matrix = readMatrixFromFile("kmeans-data.txt", &rows, &cols);

    // Ask for the number of clusters
    printf("Provide number of clusters: ");
    scanf("%d", &k);

    // Validate that k is greater than 1 and less than the number of rows
    if (k <= 1 || k > rows) {
        printf("Error: The number of clusters cannot be greater than the number of data points.\n");
        freeMatrix(matrix, rows);  // Free the matrix in case of error
        return 1;
    }

    // Apply K-means algorithm
    int* assignments = (int*)malloc(rows * sizeof(int));  // Array to store cluster assignments
    kmeansClustering(matrix, rows, cols, k, tolerance, assignments);

    // Write the clustered data to a file
    writeClusteredData("clustered_data.txt", matrix, assignments, rows, cols);

    // Free allocated memory
    free(assignments);
    freeMatrix(matrix, rows);

    return 0;
}

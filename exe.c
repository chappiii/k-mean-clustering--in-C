#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h> 


// Function to read the matrix from a file
// double** readMatrixFromFile(const char* filename, int* rows, int* cols) {
//     FILE* file = fopen(filename, "r");
//     if (file == NULL) {
//         perror("Error opening file");
//         exit(EXIT_FAILURE); 
//     }

//     *rows = 0;
//     *cols = 0;
    
//     char ch;
//     int temp_cols = 0;
//     int countingCols = 1; // Flag to count columns only for the first row

//     // Count the number of rows and columns
//     while ((ch = fgetc(file)) != EOF) {
//         if (ch == '\n') {
//             (*rows)++;
//             if (countingCols) {
//                 (*cols) = temp_cols + 1;  // Add 1 to include the last element
//                 countingCols = 0;  // Stop counting columns after the first row
//             }
//             temp_cols = 0; // Reset column counter for the next row
//         } else if (ch == ' ' || ch == '\t') {
//             temp_cols++;  // Count spaces between elements
//         }
//     }
    
//     // Add one more row if the last line doesn't end with a newline character
//     if (ch != '\n') {
//         (*rows)++;
//     }

//     // Debug: Print the number of rows and columns
//     printf("Rows: %d, Columns: %d\n", *rows, *cols);

//     // Reset file pointer to the beginning of the file
//     rewind(file);

//     // Allocate memory for the matrix (array of pointers to arrays)
//     double** matrix = (double**)malloc(*rows * sizeof(double*));
//     if (matrix == NULL) {
//         perror("Memory allocation error");
//         fclose(file);
//         exit(EXIT_FAILURE);
//     }

//     // Read the matrix from the file
//     for (int i = 0; i < *rows; i++) {
//         matrix[i] = (double*)malloc(*cols * sizeof(double));
//         if (matrix[i] == NULL) {
//             perror("Memory allocation error");
//             // Free previously allocated memory in case of error
//             for (int k = 0; k < i; k++) {
//                 free(matrix[k]);
//             }
//             free(matrix);
//             fclose(file);
//             exit(EXIT_FAILURE);
//         }

//         // Read each element in the row
//         for (int j = 0; j < *cols; j++) {
//             if (fscanf(file, "%lf", &matrix[i][j]) != 1) {
//                 fprintf(stderr, "Error reading matrix data\n");
//                 // Free allocated memory in case of error
//                 for (int k = 0; k <= i; k++) {
//                     free(matrix[k]);
//                 }
//                 free(matrix);
//                 fclose(file);
//                 exit(EXIT_FAILURE);
//             }
//         }
//     }

//     fclose(file);
    
//     // Debug: Print the matrix to verify the data
//     // printf("Matrix data:\n");
//     // for (int i = 0; i < *rows; i++) {
//     //     for (int j = 0; j < *cols; j++) {
//     //         printf("%lf ", matrix[i][j]);
//     //     }
//     //     printf("\n");
//     // }

//     return matrix; 
// }
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

// Function to select K random centroids from the matrix
double** selectRandomCentroids(double** data, int rows, int cols, int k) {
    srand(time(0));  // Initialize random seed

    // Array to keep track of selected indices (avoid selecting the same point twice)
    int* selected = (int*)calloc(rows, sizeof(int));  // Use `rows` to avoid out-of-bounds

    if (selected == NULL) {
        perror("Memory allocation error");
        exit(EXIT_FAILURE);
    }

    // Allocate memory for centroids (array of pointers to arrays)
    double** centroids = (double**)malloc(k * sizeof(double*));
    if (centroids == NULL) {
        perror("Memory allocation error");
        free(selected);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < k; i++) {
        centroids[i] = (double*)malloc(cols * sizeof(double));
        if (centroids[i] == NULL) {
            perror("Memory allocation error");
            for (int j = 0; j < i; j++) {
                free(centroids[j]);
            }
            free(centroids);
            free(selected);
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < k; i++) {
        int randomIndex;

        // Ensure that the selected index is unique
        do {
            randomIndex = rand() % rows;  // Generate a random index within the number of rows
        } while (selected[randomIndex] == 1);  // Check if already selected

        selected[randomIndex] = 1;  // Mark the index as selected

        // Copy the selected data point (centroid) to the centroids array
        for (int j = 0; j < cols; j++) {
            centroids[i][j] = data[randomIndex][j];
        }

        // Optionally, print the selected centroid
        printf("Selected centroid %d: (", i + 1);
        for (int j = 0; j < cols; j++) {
            printf("%lf", centroids[i][j]);
            if (j < cols - 1) {
                printf(", ");
            }
        }
        printf(")\n");
    }
        
    free(selected);  // Free the dynamically allocated memory for selected indices

    return centroids;  // Return the array of centroids
}

 
// Function to calculate the Euclidean distance between two points
double euclideanDistance(double* point, double* centroid, int cols) {
    double sum = 0.0;
    for (int i = 0; i < cols; i++) {
        sum += (point[i] - centroid[i]) * (point[i] - centroid[i]);
    }
    return sqrt(sum);
}

// Function to assign data points to the nearest centroid
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

// Function to free the matrix memory
void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

// Function to calculate the mean of each cluster
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


// int main() {
//     int rows, cols, k;
//     double tolerance = 0.0001;  // Set a small threshold for convergence

//     // Example data
//     double** matrix = readMatrixFromFile("data.txt", &rows, &cols);

//     // Ask for the number of clusters
//     printf("Provide number of clusters: ");
//     scanf("%d", &k);

//     // Validate that k is less than or equal to the number of rows (data points)
//     if (k > rows) {
//         printf("Error: The number of clusters cannot be greater than the number of data points.\n");
//         freeMatrix(matrix, rows);
//         return 1;
//     }

//     // Select random centroids
//     double** centroids = selectRandomCentroids(matrix, rows, cols, k);

//     // Array to store cluster assignments (which centroid each point is closest to)
//     int* assignments = (int*)malloc(rows * sizeof(int));

//     // Array to store the size of each cluster
//     int* clusterSizes = NULL;

//     // Allocate memory for old centroids to track changes between iterations
//     double** oldCentroids = (double**)malloc(k * sizeof(double*));
//     for (int i = 0; i < k; i++) {
//         oldCentroids[i] = (double*)malloc(cols * sizeof(double));
//     }

//     int converged = 0;  // Flag to indicate if centroids have converged
//     int maxIterations = 100;  // Set a maximum number of iterations to avoid infinite loop

//     for (int iter = 0; iter < maxIterations; iter++) {
//         printf("\nIteration %d:\n", iter + 1);

//         // Step 1: Copy the current centroids to oldCentroids
//         for (int i = 0; i < k; i++) {
//             for (int j = 0; j < cols; j++) {
//                 oldCentroids[i][j] = centroids[i][j];
//             }
//         }

//         // Step 2: Assign data points to clusters
//         double*** clusters = assignClusters(matrix, centroids, assignments, rows, cols, k, &clusterSizes);

//         // Print the clusters for the current iteration (optional)
//         printf("Clusters:\n");
//         for (int i = 0; i < k; i++) {
//             printf("Cluster %d: \n", i + 1);
//             for (int j = 0; j < clusterSizes[i]; j++) {
//                 printf("{ ");
//                 for (int l = 0; l < cols; l++) {
//                     printf("%lf", clusters[i][j][l]);
//                     if (l < cols - 1) printf(", ");
//                 }
//                 printf(" }\n");
//             }
//         }

//         // Step 3: Calculate the new centroids
//         calculateClusterMeans(clusters, centroids, clusterSizes, k, cols);

//         // Step 4: Check for convergence
//         converged = hasConverged(oldCentroids, centroids, k, cols, tolerance);

//         // Step 5: Free the old clusters before the next iteration
//         freeClusters(clusters, clusterSizes, k);

//         // If converged, break the loop
//         if (converged) {
//             printf("Converged after %d iterations.\n", iter + 1);
//             break;
//         }
//     }

//     // Free allocated memory
//     free(assignments);
//     freeMatrix(centroids, k);  // Free centroids
//     freeMatrix(matrix, rows);  // Free matrix
//     for (int i = 0; i < k; i++) {
//         free(oldCentroids[i]);
//     }
//     free(oldCentroids);

//     return 0;
// }

int main() {
    int rows, cols, k;
    double tolerance = 0.0001;

    // Read the data
    double** matrix = readMatrixFromFile("kmeans-data.txt", &rows, &cols);

    // Ask for the number of clusters
    printf("Provide number of clusters: ");
    scanf("%d", &k);

    // Validate that k is less than or equal to the number of rows
    if (k > rows) {
        printf("Error: The number of clusters cannot be greater than the number of data points.\n");
        freeMatrix(matrix, rows);
        return 1;
    }

    // Select random centroids
    double** centroids = selectRandomCentroids(matrix, rows, cols, k);

    // Array to store cluster assignments
    int* assignments = (int*)malloc(rows * sizeof(int));

    // Array to store cluster sizes
    int* clusterSizes = NULL;

    // Allocate memory for old centroids
    double** oldCentroids = (double**)malloc(k * sizeof(double*));
    for (int i = 0; i < k; i++) {
        oldCentroids[i] = (double*)malloc(cols * sizeof(double));
    }

    int converged = 0;
    int maxIterations = 100;

    for (int iter = 0; iter < maxIterations; iter++) {
        printf("\nIteration %d:\n", iter + 1);

        // Copy current centroids to oldCentroids
        for (int i = 0; i < k; i++) {
            for (int j = 0; j < cols; j++) {
                oldCentroids[i][j] = centroids[i][j];
            }
        }

        // Assign data points to clusters
        double*** clusters = assignClusters(matrix, centroids, assignments, rows, cols, k, &clusterSizes);

        // Calculate the new centroids
        calculateClusterMeans(clusters, centroids, clusterSizes, k, cols);

        // Check for convergence
        converged = hasConverged(oldCentroids, centroids, k, cols, tolerance);

        // Free the old clusters before the next iteration
        freeClusters(clusters, clusterSizes, k);

        // If converged, break the loop
        if (converged) {
            printf("Converged after %d iterations.\n", iter + 1);
            break;
        }
    }

    // Output the original data points with cluster assignments
    printf("\nClustered Data:\n");
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%lf ", matrix[i][j]);
        }
        printf("-> Cluster %d\n", assignments[i] + 1);  // Add 1 to make it human-readable
    }

    // Write clustered data to a file
    writeClusteredData("clustered_data.txt", matrix, assignments, rows, cols);

    // Free allocated memory
    free(assignments);
    freeMatrix(centroids, k);
    freeMatrix(matrix, rows);
    for (int i = 0; i < k; i++) {
        free(oldCentroids[i]);
    }
    free(oldCentroids);

    return 0;
}
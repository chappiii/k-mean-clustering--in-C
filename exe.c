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
    int countingCols = 1; // Flag to count columns only for the first row

    // Count the number of rows and columns
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            (*rows)++;
            if (countingCols) {
                (*cols) = temp_cols + 1;  // Add 1 to include the last element
                countingCols = 0;  // Stop counting columns after the first row
            }
            temp_cols = 0; // Reset column counter for the next row
        } else if (ch == ' ' || ch == '\t') {
            temp_cols++;  // Count spaces between elements
        }
    }
    
    // Add one more row if the last line doesn't end with a newline character
    if (ch != '\n') {
        (*rows)++;
    }

    // Debug: Print the number of rows and columns
    printf("Rows: %d, Columns: %d\n", *rows, *cols);

    // Reset file pointer to the beginning of the file
    rewind(file);

    // Allocate memory for the matrix (array of pointers to arrays)
    double** matrix = (double**)malloc(*rows * sizeof(double*));
    if (matrix == NULL) {
        perror("Memory allocation error");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    // Read the matrix from the file
    for (int i = 0; i < *rows; i++) {
        matrix[i] = (double*)malloc(*cols * sizeof(double));
        if (matrix[i] == NULL) {
            perror("Memory allocation error");
            // Free previously allocated memory in case of error
            for (int k = 0; k < i; k++) {
                free(matrix[k]);
            }
            free(matrix);
            fclose(file);
            exit(EXIT_FAILURE);
        }

        // Read each element in the row
        for (int j = 0; j < *cols; j++) {
            if (fscanf(file, "%lf", &matrix[i][j]) != 1) {
                fprintf(stderr, "Error reading matrix data\n");
                // Free allocated memory in case of error
                for (int k = 0; k <= i; k++) {
                    free(matrix[k]);
                }
                free(matrix);
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
    
    // Debug: Print the matrix to verify the data
    // printf("Matrix data:\n");
    // for (int i = 0; i < *rows; i++) {
    //     for (int j = 0; j < *cols; j++) {
    //         printf("%lf ", matrix[i][j]);
    //     }
    //     printf("\n");
    // }

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
void assignClusters(double** data, double** centroids, int* assignments, int rows, int cols, int k) {
    for (int i = 0; i < rows; i++) {  // Loop over each data point
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
    }
}


// Function to free the matrix memory
void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int rows, cols, k;

    // Read matrix from file
    double** matrix = readMatrixFromFile("sample.txt", &rows, &cols);

    // Ask for the number of clusters
    printf("Provide number of clusters: ");
    scanf("%d", &k);

    // Validate that k is less than or equal to the number of rows (data points)
    if (k > rows) {
        printf("Error: The number of clusters cannot be greater than the number of data points.\n");
        freeMatrix(matrix, rows);
        return 1;
    }

    // Select random centroids
    double** centroids = selectRandomCentroids(matrix, rows, cols, k);

    // Array to store cluster assignments (which centroid each point is closest to)
    int* assignments = (int*)malloc(rows * sizeof(int));

    // Assign data points to the nearest centroid
    assignClusters(matrix, centroids, assignments, rows, cols, k);

    // Output the cluster assignments
    printf("Cluster assignments:\n");
    for (int i = 0; i < rows; i++) {
        printf("Data point %d is assigned to cluster %d\n", i + 1, assignments[i] + 1);  // Adding 1 for human-readable index
    }

    // Free allocated memory
    free(assignments);
    freeMatrix(centroids, k);  // Free centroids
    freeMatrix(matrix, rows);  // Free matrix

    return 0;
}


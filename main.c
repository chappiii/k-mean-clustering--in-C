#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define TOLERANCE 0.0001


// Function prototypes
double** readMatrixFromFile(const char* filename, int* rows, int* cols);
double** selectRandomCentroids(double** data, int rows, int cols, int k);
bool areCentroidsEqual(double* centroid1, double* centroid2, int cols);
void kmeansClustering(double** data, int rows, int cols, int k, double tolerance, int* assignments, double** centroids);
void writeClusteredFile(const char* filename, double** data, int* assignments, int rows, int cols);
void freeMatrix(double** matrix, int rows);


int main() {
    int rows, cols, k, choice;
    char* filename = NULL;
    bool validChoice = false;

    // Ask user to choose between large data and the given data
    do {
        // Display the menu for dataset selection
        printf("Select the dataset to use:\n");
        printf("1. kmeans-data.txt\n");
        printf("2. large-data.txt\n");
        printf("3. Exit\n");
        printf("Enter your choice (1, 2 or 3: ");

        // Check if input is a valid integer
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a numeric choice (1, 2, 3, or 4).\n");

            while (getchar() != '\n');
            continue;
        }

        // Handle valid choices
        switch (choice) {
            case 1:
                filename = "kmeans-data.txt";
                validChoice = true;
                break;
            case 2:
                filename = "large-data.txt";
                validChoice = true;
                break;
            case 3:
                printf("Exiting the program.\n");
                return 0;
            default:
                printf("Invalid choice. Please enter 1, 2, 3, or 4.\n");
                break;
        }
    } while (!validChoice);

    // Read the data from the selected file
    double** matrix = readMatrixFromFile(filename, &rows, &cols);

    // Ask user for the number of clusters
    do {
        printf("Provide number of clusters (must be greater than or equal to 1 and less than or equal to the number of data points): ");

        // Check if input is a valid integer
        if (scanf("%d", &k) != 1) {
            printf("Error: Invalid input. Please enter an integer value.\n");

            while (getchar() != '\n');
            k = -1;
            continue;
        }

        // Check if the value of k is within the valid range
        if (k < 1 || k > rows) {
            printf("Error: The number of clusters must be greater than or equal to 1 and less than or equal to the number of data points.\n");
        }
    } while (k < 1 || k > rows);

    double** centroids = NULL;

    // Ask user how to initialize centroids
    do {
        // Display the menu for centroids initialization
        printf("Choose centroid initialization method:\n");
        printf("1. Randomly select centroids\n");
        printf("2. Enter centroids manually\n");
        printf("3. Exit\n");
        printf("Enter your choice (1, 2, or 3): ");

        // Check if input is a valid integer
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a numeric choice (1, 2, or 3).\n");

            while (getchar() != '\n');
            continue;
        }

        // Handle valid choices
        if (choice == 1) {
            centroids = selectRandomCentroids(matrix, rows, cols, k);
            break;
        } else if (choice == 2) {
            centroids = (double**)malloc(k * sizeof(double*));
            for (int i = 0; i < k; i++) {
                centroids[i] = (double*)malloc(cols * sizeof(double));
                bool valid = false;

                // Loop until a valid, unique centroid is entered
                while (!valid) {
                    printf("Enter coordinates for centroid %d (separated by space):\n", i + 1);
                    valid = true;
                    for (int j = 0; j < cols; j++) {
                        // Check if input is valid double
                        if (scanf("%lf", &centroids[i][j]) != 1) {
                            printf("Invalid input. Please enter numeric coordinates for the centroid.\n");
                            valid = false;

                            while (getchar() != '\n');
                            break;
                        }
                    }

                    // If all inputs are valid, check for uniqueness
                    if (valid) {
                        for (int j = 0; j < i; j++) {
                            if (areCentroidsEqual(centroids[i], centroids[j], cols)) {
                                printf("Error: Centroid %d is equal to Centroid %d. Please enter distinct centroids.\n", i + 1, j + 1);
                                valid = false;
                                break;
                            }
                        }
                    }
                }
            }
            break;
        } else if (choice == 3) {
            printf("Exiting the program.\n");
            freeMatrix(matrix, rows);
            return 0;
        } else {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    } while (choice != 3);

    // Apply K-means algorithm
    int* assignments = (int*)malloc(rows * sizeof(int));  // Array to store cluster assignments
    kmeansClustering(matrix, rows, cols, k, TOLERANCE, assignments, centroids);

    // Write the clustered data to a file
    writeClusteredFile("kmeans-output.txt", matrix, assignments, rows, cols);

    // Free allocated memory
    free(assignments);
    freeMatrix(matrix, rows);
    freeMatrix(centroids, k);

    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Function prototypes
double** readMatrixFromFile(const char* filename, int* rows, int* cols);
void freeMatrix(double** matrix, int rows);
double** selectRandomCentroids(double** data, int rows, int cols, int k);
void kmeansClustering(double** data, int rows, int cols, int k, double tolerance, int* assignments, double** centroids);
void writeClusteredFile(const char* filename, double** data, int* assignments, int rows, int cols);
bool areCentroidsEqual(double* centroid1, double* centroid2, int cols);

// Function to compare two centroids to see if they are equal
bool areCentroidsEqual(double* centroid1, double* centroid2, int cols) {
    for (int i = 0; i < cols; i++) {
        if (centroid1[i] != centroid2[i]) {
            return false;  // Centroids are different
        }
    }
    return true;  // Centroids are equal
}

int main() {
    int rows, cols, k, choice;
    double tolerance = 0.0001;
    char* filename = NULL;  // Pointer to hold the file name
    bool validChoice = false;

    // Continuously prompt the user until a valid choice is made or they exit
    do {
        // Display the menu for dataset selection
        printf("Select the dataset to use:\n");
        printf("1. kmeans-data.txt\n");
        printf("2. small-data.txt\n");
        printf("3. large-data.txt\n");
        printf("4. Exit\n");
        printf("Enter your choice (1, 2, 3, or 4): ");
        scanf("%d", &choice);

        // Set the filename based on user selection or handle invalid choices
        switch (choice) {
            case 1:
                filename = "kmeans-data.txt";
                validChoice = true;  // Valid choice, break the loop
                break;
            case 2:
                filename = "small-data.txt";
                validChoice = true;  // Valid choice, break the loop
                break;
            case 3:
                filename = "another-data.txt";
                validChoice = true;  // Valid choice, break the loop
                break;
            case 4:
                printf("Exiting the program.\n");
                return 0;  // Exit the program
            default:
                printf("Invalid choice. Please enter 1, 2, 3, or 4.\n");
                break;
        }
    } while (!validChoice);

    // Read the data from the selected file
    double** matrix = readMatrixFromFile(filename, &rows, &cols);

    // Ask for the number of clusters
    do {
        printf("Provide number of clusters (must be greater than 1 and less than or equal to the number of data points): ");
        if (scanf("%d", &k) != 1) { // Check if input is a valid integer
            printf("Error: Invalid input. Please enter an integer value.\n");
            while (getchar() != '\n'); // Clear the input buffer
            continue; // Re-prompt for input
        }

        if (k <= 1 || k > rows) {
            printf("Error: The number of clusters must be greater than 1 and less than or equal to the number of data points.\n");
        }
    } while (k <= 1 || k > rows);

    double** centroids = NULL;
    // Ask user how to initialize centroids
    do {
        // Prompt user for choice
        printf("Choose centroid initialization method:\n");
        printf("1. Randomly select centroids\n");
        printf("2. Enter centroids manually\n");
        printf("3. Exit\n");
        printf("Enter your choice (1, 2, or 3): ");
        scanf("%d", &choice);

        // Handle valid choices
        if (choice == 1) {
            centroids = selectRandomCentroids(matrix, rows, cols, k);
            break;  // Exit the loop after handling valid input
        } else if (choice == 2) {
            centroids = (double**)malloc(k * sizeof(double*));
            for (int i = 0; i < k; i++) {
                centroids[i] = (double*)malloc(cols * sizeof(double));
                bool valid = false;

                // Loop until a valid, unique centroid is entered
                while (!valid) {
                    printf("Enter coordinates for centroid %d (separated by space):\n", i + 1);
                    for (int j = 0; j < cols; j++) {
                        scanf("%lf", &centroids[i][j]);
                    }

                    valid = true;  // Assume valid for now

                    // Check against previously entered centroids
                    for (int j = 0; j < i; j++) {
                        if (areCentroidsEqual(centroids[i], centroids[j], cols)) {
                            printf("Error: Centroid %d is equal to Centroid %d. Please enter distinct centroids.\n", i + 1, j + 1);
                            valid = false;  // Set valid to false, as centroids are not unique
                            break;
                        }
                    }
                }
            }
            break;  // Exit the loop after handling valid input
        } else if (choice == 3) {
            // Exit the program
            printf("Exiting the program.\n");
            freeMatrix(matrix, rows); // Clean up any dynamically allocated memory (if necessary)
            return 0; // Exit the program
        } else {
            // Invalid choice
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }
    } while (choice != 3);

    // Apply K-means algorithm
    int* assignments = (int*)malloc(rows * sizeof(int));  // Array to store cluster assignments
    kmeansClustering(matrix, rows, cols, k, tolerance, assignments, centroids);

    // Write the clustered data to a file
    writeClusteredFile("kmeans-output.txt", matrix, assignments, rows, cols);

    // Free allocated memory
    free(assignments);
    freeMatrix(matrix, rows);
    freeMatrix(centroids, k);

    return 0;
}

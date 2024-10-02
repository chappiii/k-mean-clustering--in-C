#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void selectRandomPoints(double** data, int rows, int cols, int k) {
    srand(time(0));  // Initialize random seed

    // Array to keep track of selected indices (avoid selecting the same point twice)
    int* selected = (int*)calloc(k, sizeof(int));

    for (int i = 0; i < k; i++) {
        int randomIndex;

        // Ensure that the selected index is unique
        do {
            randomIndex = rand() % rows;  // Generate a random index within the number of rows
        } while (selected[randomIndex] == 1);  // Check if already selected

        selected[randomIndex] = 1;  // Mark the index as selected

        // Print the randomly selected data point
        printf("Selected point %d: (", randomIndex);
        for (int j = 0; j < cols; j++) {
            printf("%lf", data[randomIndex][j]);
            if (j < cols - 1) {
                printf(", ");
            }
        }
        printf(")\n");
    }

    free(selected);  // Free the dynamically allocated memory
}

double euclideanDistance(double* point1, double* point2, int size) {
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        double diff = point1[i] - point2[i];
        sum += diff * diff;
    }
    return sqrt(sum);
}

double** calculateDistanceMatrix(double** matrix, int rows, int cols) {
    double** distanceMatrix = (double**)malloc(rows * sizeof(double*));
    for (int i = 0; i < rows; i++) {
        distanceMatrix[i] = (double*)malloc(rows * sizeof(double));
        for (int j = 0; j < rows; j++) {
            if (i == j) {
                distanceMatrix[i][j] = 0.0; // Distance from a point to itself is zero
            } else {
                distanceMatrix[i][j] = euclideanDistance(matrix[i], matrix[j], cols);
            }
        }
    }
    return distanceMatrix;
}

void findCentroid(int points[][2], int numPoints, float *centroidX, float *centroidY) {
    int sumX = 0, sumY = 0;
    for (int i = 0; i < numPoints; i++) {
        sumX += points[i][0];
        sumY += points[i][1];
    }
    *centroidX = (float)sumX / numPoints;
    *centroidY = (float)sumY / numPoints;
}

void kmean(double** matrix, int Kcluster,){

}
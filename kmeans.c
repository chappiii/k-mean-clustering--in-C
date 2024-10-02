#include <stdio.h>
#include <stdlib.h>
#include <string.h>


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
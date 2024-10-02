#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** readMatrixFromFile(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE); // Exit if file opening fails
    }

    // Read the number of rows and columns
    fscanf(file, "%d %d", rows, cols);

    // Allocate memory for the matrix (array of pointers to arrays)
    double** matrix = (double**)malloc(*rows * sizeof(double*));
    for (int i = 0; i < *rows; i++) {
        matrix[i] = (double*)malloc(*cols * sizeof(double));
        
        // Read each element in the row
        for (int j = 0; j < *cols; j++) {
            fscanf(file, "%lf", &matrix[i][j]);
        }
    }
    fclose(file);
    return matrix; // Return the pointer to the matrix
}

int main(){
    int rows, cols;
    double** matrix = readMatrixFromFile("data.txt", &rows, &cols);

    // printf("%f", matrix);
    // printf("%d %d \n", rows, cols);
}
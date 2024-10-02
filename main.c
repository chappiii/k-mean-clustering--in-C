#include <stdio.h>
#include <stdlib.h>
#include <string.h>

double** readMatrixFromFile(const char* filename, int* rows, int* cols) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE); // Exit if file opening fails
    }

    // Initialize rows and cols
    *rows = 0;
    *cols = 0;
    
    char ch;
    int temp_cols = 0;
    
    // Count the number of rows and columns
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            (*rows)++;
            if (*cols == 0) {
                *cols = temp_cols; // Set columns after reading the first line
            }
        } else if (ch == ' ') {
            temp_cols++; // Count number of spaces in the first row
        }
    }
    
    // Add one to cols since there are cols+1 numbers in each row
    *cols = *cols + 1;

    // Reset file pointer to beginning of file
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
    return matrix; // Return the pointer to the matrix
}

int main(){
    int rows, cols, k;
    
    // Call the function to read matrix and get rows and cols
    // double** matrix = readMatrixFromFile("kmeans-data.txt", &rows, &cols);

    // Display the number of rows and columns
    // printf("Number of rows: %d\n", rows);
    // printf("Number of columns: %d\n", cols);

    printf("provide number of clusters :");
    scanf("%d", &k);
    printf("%d \n", k);


    // Optionally, you can free the matrix after use if not needed further
    // for (int i = 0; i < rows; i++) {
    //     free(matrix[i]);
    // }
    // free(matrix);

    return 0;
}
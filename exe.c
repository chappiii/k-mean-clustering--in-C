#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
    
    // Count the number of rows and columns
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            (*rows)++;
            if (*cols == 0) {
                *cols = temp_cols; 
            }
        } else if (ch == ' ') {
            temp_cols++;
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
    return matrix; 
}

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

void freeMatrix(double** matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main(){
    int rows, cols, k;
    
    double** matrix = readMatrixFromFile("kmeans-data.txt", &rows, &cols);

    printf("provide number of clusters :");
    scanf("%d", &k);

    selectRandomPoints(matrix, rows, cols, k);

    freeMatrix(matrix, rows);

    return 0;
}
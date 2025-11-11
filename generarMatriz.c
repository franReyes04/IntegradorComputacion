#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    int rows, cols, max;
    FILE *file;
    printf("Ingrese número de filas: ");
    scanf("%d", &rows);
    printf("Ingrese número de columnas: ");
    scanf("%d", &cols);
    printf("Ingrese el número máximo a generar: ");
    scanf("%d", &max);
    file = fopen("matriz.txt", "w");
    if (file == NULL) {
        printf("Error: could not create the file.\n");
        return 1;
    }
    srand(time(NULL));
    fprintf(file, "%d %d\n", rows, cols);
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            int num = rand() % max + 1;
            fprintf(file, "%d ", num);
        }
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Matriz guardada en matriz.txt exitosamente.\n");
    return 0;
}


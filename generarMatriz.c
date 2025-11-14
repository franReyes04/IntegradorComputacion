#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
    int rows, cols, max;
    FILE *file;
    // Se pide el ingreso por consola de las dimensiones deseadas,
    // y del rango máximo de números
    printf("Ingrese número de filas: ");
    scanf("%d", &rows);
    printf("Ingrese número de columnas: ");
    scanf("%d", &cols);
    printf("Ingrese el número máximo a generar: ");
    scanf("%d", &max);
    // Abrir archivo matrix.txt en modo escritura
    file = fopen("matriz.txt", "w");
    // Manejo de errores
    if (file == NULL) {
        printf("Error: no se pudo crear el archivo.\n");
        return 1;
    }
    // Se usa el tiempo actual de la computadora para la semilla
    // del generador de numeros aleatorios, de esta manera nunca
    // se generará dos veces la misma matriz
    srand(time(NULL));

    fprintf(file, "%d %d\n", rows, cols); // Escribe las dimensiones en la primera línea del archivo

    int i, j;
    // Iteración entre todas la celdas de la matriz
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            // Generar un número aleatorio entre 0 y max
            int num = rand() % max + 1;
            // Imprimirlo en el archivo
            fprintf(file, "%d ", num);
        }
        // Al final de la fila imprime el salto de línea
        fprintf(file, "\n");
    }
    fclose(file);
    printf("Matriz guardada en matriz.txt exitosamente.\n");
    return 0;
}


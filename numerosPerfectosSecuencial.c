#include <stdio.h>
#include <stdlib.h>
#include <time.h>
// Funcion que devuelve 1 si un número es perfecto 
// y 0 si no lo es
int isPerfect(int n){
    // La suma se inicializa en 1 porque 1 es divisor de todos los números
    int i, sum = 1;
    // Iterar entre 2 y la raíz cuadrada de n
    for (i = 2; i * i <= n; i++){
        // Comprobar que i sea divisor de n
        if (n % i == 0){
            // Si i todavía no llega a la raíz cuadrada de n 
            // se suma tanto i como el divisor complementario a i
            // que es igual a n/i
            // Por ejemplo: si n=6, 2 es divisor de n por lo tanto se acumula 2
            // y también 3, ya que 2 * 3 = n, no hace falta volver a comprobar 
            // si 3 es divisor de 6
            if (i * i != n)
                sum = sum + i + n / i;
            else
                // si i es la raíz de n el divisor complementario n/i es igual
                // a i, no hay que sumar los dos porque si lo hicieramos
                // estaríamos acumulando dos veces el mismo divisor
                sum = sum + i;
        }
    }
    // Comprobar que la suma sea igual a n y que n no sea 1
    if (sum == n && n != 1) return 1;
    return 0;
}
int main(int argc, char **argv){
    __clock_t start, end;
    // Inicia la medición de tiempo
    start = clock();
    // Abrir el archivo matriz.txt en modo lectura
    FILE *archivoMatriz;
    archivoMatriz = fopen("matriz.txt", "r");
    if (archivoMatriz == NULL) {
        printf("Error abriendo el archivo!\n");
        return 1;
    }
    int sum = 0;
    // Leer uno por uno los números de la matriz y verificar si es perfecto
    int number;
    while (fscanf(archivoMatriz, "%d", &number) == 1) {
        sum += isPerfect(number);
    }
    // Cerrar el archivo
    fclose(archivoMatriz);
    // Mostrar resultados de la ejecución
    printf("Cantidad de números perfectos: %d\n", sum);
    end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos\n", time_taken);
    return 0;
}

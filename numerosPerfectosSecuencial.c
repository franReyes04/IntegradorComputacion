#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int isPerfect(int n){
    int i, sum = 1;
    for (i = 2; i * i <= n; i++){
        if (n % i == 0)
        {
            if (i * i != n)
                sum = sum + i + n / i;
            else
                sum = sum + i;
        }
    }
    if (sum == n && n != 1)return 1;
    return 0;
}
int main(int argc, char **argv){
    __clock_t start, end;
    start = clock();
    FILE *archivoMatriz;
    archivoMatriz = fopen("matriz.txt", "r");
    if (archivoMatriz == NULL) {
        printf("Error abriendo el archivo!\n");
        return 1;
    }
    int sum = 0;
    int number;
    while (fscanf(archivoMatriz, "%d", &number) == 1) {
        sum += isPerfect(number);
    }
    fclose(archivoMatriz);
    printf("Cantidad de números perfectos: %d\n", sum);
    end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo de ejecución: %f segundos\n", time_taken);
    FILE *archivoTiempo;
    archivoTiempo = fopen("tiemposSecuencial.txt", "a");
    if (archivoTiempo == NULL) {
        printf("Error abriendo el archivo de tiempos!\n");
        return 1;
    }
    fprintf(archivoTiempo, "%f\n", time_taken);
    fclose(archivoTiempo);
    return 0;
}

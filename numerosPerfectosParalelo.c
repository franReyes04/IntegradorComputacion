#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
int isPerfect(int n){
    int i, sum = 1;
    for (i = 2; i * i <= n; i++){
        if (n % i == 0){
            if (i * i != n) sum = sum + i + n / i;
            else sum = sum + i;
        }
    }
    if (sum == n && n != 1)
        return 1;
    return 0;
}
int main(int argc, char **argv){
    int size, my_id, sum=0, number, rows, cols, total_sum=0;
    double start, end;
    char buffer[6000];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    start = MPI_Wtime();
    FILE *archivoMatriz;
    archivoMatriz = fopen("matriz.txt", "r");
    if (archivoMatriz == NULL) {
        printf("Error abriendo el archivo!\n");
        return 1;
    }
    fscanf(archivoMatriz, "%d %d", &rows, &cols);
    int rows_per_node = rows / size;
    int remainder = rows % size;

    int my_rows = rows_per_node + (my_id < remainder ? 1 : 0);
    int start_row = my_id * rows_per_node + (my_id < remainder ? my_id : remainder);
    int i;
    for (i = 0; i < start_row * cols; i++) {
        int tmp;
        fscanf(archivoMatriz, "%d", &tmp);
    }
    int *submatrix = malloc(my_rows * cols * sizeof(int));
    int j, k;
    for (i = 0; i < my_rows * cols; i++) {
        if (fscanf(archivoMatriz, "%d", &submatrix[i]) != 1) {
            printf("Nodo %d: error al leer dato %d.\n", my_id, i);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    fclose(archivoMatriz);
    for (k = 0; k < my_rows; k++) {
        for (j = 0; j < cols; j++) {
            sum += isPerfect(submatrix[k * cols + j]);
        }
    }
    free(submatrix);
    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    end = MPI_Wtime();
    double time_taken = end - start;
    if (!my_id) {
        printf("Cantidad de números perfectos: %d\n", total_sum);
        printf("Tiempo de ejecución: %f segundos\n", time_taken);
        FILE *archivoTiempo;
        archivoTiempo = fopen("tiemposParalelo.txt", "a");
        if (archivoTiempo == NULL) {
            printf("Error abriendo el archivo de tiempos!\n");
            return 1;
        }
        fprintf(archivoTiempo, "Nodo 0: %f\n", time_taken);
        for(i = 1; i < size; i++){
            double node_time;
            MPI_Recv(&node_time, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fprintf(archivoTiempo, "Nodo %d: %f\n", i, node_time);
        }
        fclose(archivoTiempo);
    }else{
        MPI_Send(&time_taken, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}

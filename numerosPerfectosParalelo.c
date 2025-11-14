#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
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
    int size, my_id, sum=0, number, rows, cols, total_sum=0;
    double start, end;
    // Inicializar MPI, obtener el número de nodos y my_id
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    // Iniciar la medición de tiempo
    start = MPI_Wtime();
    
    // Todos los nodos abren el archivo matriz.txt en modo lectura
    FILE *archivoMatriz;
    archivoMatriz = fopen("matriz.txt", "r");
    if (archivoMatriz == NULL) {
        printf("Error abriendo el archivo!\n");
        return 1;
    }
    // Se leen las primera 2 líneas que contienen las dimensiones de la matriz
    // para repartir el trabajo
    fscanf(archivoMatriz, "%d %d", &rows, &cols);
    // Dividir por fila
    int rows_per_node = rows / size;
    int remainder = rows % size;
    // Si sobran filas se le asigna una fila extra a los primeros nodos hasta
    // que no queden sobrantes
    int my_rows = rows_per_node + (my_id < remainder ? 1 : 0);
    int start_row = my_id * rows_per_node + (my_id < remainder ? my_id : remainder);

    // Cada nodo lee el archivo matriz.txt hasta quedar parado 
    // en la fila que le corresponde procesar
    int i;
    for (i = 0; i < start_row * cols; i++) {
        int tmp;
        fscanf(archivoMatriz, "%d", &tmp);
    }
    // Reservar espacio en memoria para la porción de la matriz a procesar
    int *submatrix = malloc(my_rows * cols * sizeof(int));
    // Guardar los números del archivo en la submatriz
    for (i = 0; i < my_rows * cols; i++) {
        if (fscanf(archivoMatriz, "%d", &submatrix[i]) != 1) {
            printf("Nodo %d: error al leer dato %d.\n", my_id, i);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }
    fclose(archivoMatriz);
    int j, k;
    // Se itera por la submatriz para obtener la cantidad de números perfectos
    for (k = 0; k < my_rows; k++) {
        for (j = 0; j < cols; j++) {
            sum += isPerfect(submatrix[k * cols + j]);
        }
    // Al terminar cada nodo libera el espacio de memoria reservado
    free(submatrix);
    // Se juntan las sumas parciales de cada proceso en el nodo 0
    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    // Se finaliza la medicion de tiempo
    end = MPI_Wtime();
    double time_taken = end - start;
    if (!my_id) {
        // El nodo 0 imprime los resultados
        printf("Cantidad de números perfectos: %d\n", total_sum);
        printf("Tiempo de ejecución: %f segundos\n", time_taken);
        FILE *archivoTiempo;
        // En un archivo se guardan los resultados
        archivoTiempo = fopen("tiemposParalelo.txt", "a");
        if (archivoTiempo == NULL) {
            printf("Error abriendo el archivo de tiempos!\n");
            return 1;
        }
        fprintf(archivoTiempo, "Nodo 0: %f\n", time_taken);
        // Se guarda en el archivo el tiempo de cada proceso
        for(i = 1; i < size; i++){
            double node_time;
            MPI_Recv(&node_time, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            fprintf(archivoTiempo, "Nodo %d: %f\n", i, node_time);
        }
        fclose(archivoTiempo);
    }else{
        // Cada nodo envía su tiempo individual para medir el balanceo de carga
        MPI_Send(&time_taken, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <string.h>


// Función que inicializa la cadena de longitud n.
void inicializaCadena(char *cadena, int n) {
    int i;
    for (i = 0; i < n / 2; i++) {
        cadena[i] = 'A';
    }
    for (i = n / 2; i < 3 * n / 4; i++) {
        cadena[i] = 'C';
    }
    for (i = 3 * n / 4; i < 9 * n / 10; i++) {
        cadena[i] = 'G';
    }
    for (i = 9 * n / 10; i < n; i++) {
        cadena[i] = 'T';
    }
}

int main(int argc, char **argv) {
    // Si no tenemos 3 argumentos salimos del programa.
    if (argc != 3) {
        printf("Número incorrecto de parámetros\nLa sintaxis debe ser: program n L\n"
               "  program es el nombre del ejecutable\n  n es el tamaño de la cadena a generar\n"
               "  L es la letra de la que se quiere contar apariciones (A, C, G o T)\n");
        exit(1);
    }
    //Variables necesarias.
    int rank, size, i, n, count = 0, local_count = 0;
    // Letra a contar.
    char letter = *argv[2];
    // Longitud de la cadena e inicialización.
    n = atoi(argv[1]);
    char *str = (char *) malloc(n * sizeof(char));
    inicializaCadena(str, n);

    // Iniciamos el MPI (Message Passing Interface).
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Dividir la cadena en trozos iguales.
    int chunk_size = n / size;
    int last_chunk_size = n - (size - 1) * chunk_size;
    if (rank == size - 1) {
        chunk_size = last_chunk_size;
    }
    char *local_str = malloc(chunk_size * sizeof(char));

    if(rank==0){
        printf("Letra buscada: %c\n",letter);
        printf("Cadena: %s\n",str);
    }

    // Enviamos los trozos de la cadena a cada proceso utilizando MPI_Scatter.
    MPI_Scatter(str, chunk_size, MPI_CHAR, local_str, chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Para ver que se envia bn.
    printf("Rank:%d Chunk_size:%d \n",rank,chunk_size);

    // Contar las repeticiones de la letra en el trozo de la cadena correspondiente a este proceso.
    for (i = 0; i < chunk_size; i++) {
        if (local_str[i] == letter) {
            local_count++;
        }
    }

    // Reducir los resultados de cada proceso utilizando MPI_Reduce.
    MPI_Reduce(&local_count, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Si es el proceso 0, se muestra el resultado final.
    if (rank == 0) {
        printf("La letra '%c' aparece %d veces en la cadena.\n", letter, count);
    }
    MPI_Finalize();
    free(str);
    free(local_str);

    return 0;
}
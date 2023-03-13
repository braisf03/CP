#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>
#include <string.h>

// Funcion que inicializa la cadena de longitud n.
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
    if(argc != 3){
        printf("Numero incorrecto de parametros\nLa sintaxis debe ser: program n L\n"
               "  program es el nombre del ejecutable\n  n es el tamaño de la cadena a generar\n"
               "  L es la letra de la que se quiere contar apariciones (A, C, G o T)\n");
        exit(1);
    }
    //Variables necesarias.
    int rank, size, i, n, count, local_count = 0;
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
    char *local_str = malloc(chunk_size);

    // Enviar los trozos de la cadena a cada proceso.
    if (rank == 0) { // Si el proceso es el 0, se envia cada trozo de la cadena a cada MPI.
        for (i = 0; i < size; i++) {
            strncpy(local_str, str + i * chunk_size, chunk_size);
            MPI_Send(local_str, chunk_size, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    }

    // Recibir el trozo de la cadena correspondiente a este proceso.
    MPI_Recv(local_str, chunk_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Esto es para comprobar qué se envía a cada proceso.
    printf("Rank:%d Chunk_size:%d Str:%c%c\n",rank,chunk_size,local_str[0],local_str[1]);


    // Contar las repeticiones de la letra en el trozo de la cadena correspondiente a este proceso.
    for (i = 0; i < chunk_size; i++) {
        if (local_str[i] == letter) {
            local_count++;
        }
    }


    // Envia el resultado al proceso 0, y si es el proceso 0, se reciben todas las soluciones de los demás procesos.
    if (rank != 0) {
        MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        // Esta línea es importante ya que si no se hace se pierde la cuenta hecha por el proceso 0.
        count = local_count;
        for (i = 1; i < size; i++) {
            MPI_Recv(&local_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            count += local_count;
        }
        // Espera tiempo para que se printee mas tarde.
        for (int j = 0; j < 10; ++j);
        printf("%s\n",str);
        printf("La letra '%c' aparece %d veces en la cadena.\n", letter, count);
    }

    // Se liberan todos los malloc y se cierra el MPI.
    MPI_Finalize();
    free(str);
    free(local_str);
    return 0;
}

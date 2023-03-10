#include <stdio.h>
#include <stdlib.h>
#include <mpi/mpi.h>

void inicializaCadena(char *cadena, int n){
    int i;
    for(i=0; i<n/2; i++){
        cadena[i] = 'A';
    }
    for(i=n/2; i<3*n/4; i++){
        cadena[i] = 'C';
    }
    for(i=3*n/4; i<9*n/10; i++){
        cadena[i] = 'G';
    }
    for(i=9*n/10; i<n; i++){
        cadena[i] = 'T';
    }
}

int main(int argc, char *argv[]){
    int i, n, count, local_count=0;
    char *cadena;
    char L;

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if(rank == 0){
        if(argc != 3){
            printf("Numero incorrecto de parametros\nLa sintaxis debe ser: program n L\n"
                   "  program es el nombre del ejecutable\n  n es el tamaño de la cadena a generar\n"
                   "  L es la letra de la que se quiere contar apariciones (A, C, G o T)\n");
            exit(1);
        }

        n = atoi(argv[1]);
        L = *argv[2];

        for(i=1; i<size; i++){
            MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&L, 1, MPI_CHAR, i, 0, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&L, 1, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    cadena = (char *) malloc(n*sizeof(char));
    inicializaCadena(cadena, n);

    for(i=rank; i<n; i+=size){
        if(cadena[i] == L){
            local_count++;
        }
    }

    if(rank != 0){
        MPI_Send(&local_count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    } else {
        count = local_count;
        for(i=1; i<size; i++){
            int recv_count;
            MPI_Recv(&recv_count, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            count += recv_count;
        }
        printf("El numero total de apariciones de la letra %c es %d\n", L, count);
    }
    free(cadena);
    MPI_Finalize();
    exit(0);
}
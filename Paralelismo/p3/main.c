#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <mpi/mpi.h>

#define DEBUG 0

/* Translation of the DNA bases
   A -> 0
   C -> 1
   G -> 2
   T -> 3
   N -> 4*/


#define M  1000000 // Number of sequences
#define N  200  // Number of bases per sequence

// The distance between two bases
int base_distance(int base1, int base2) {

    if ((base1 == 4) || (base2 == 4)) {
        return 3;
    }

    if (base1 == base2) {
        return 0;
    }

    if ((base1 == 0) && (base2 == 3)) {
        return 1;
    }

    if ((base2 == 0) && (base1 == 3)) {
        return 1;
    }

    if ((base1 == 1) && (base2 == 2)) {
        return 1;
    }

    if ((base2 == 2) && (base1 == 1)) {
        return 1;
    }

    return 2;
}

int main(int argc, char *argv[]) {

    // Variables necesarias
    int i, j, numprocs, rank;
    int *data1, *data2;
    int *result, *local_result;
    int *local_data1, *local_data2;
    int *tam, *despl;
    struct timeval temp_comp1, temp_comp2, temp_transf1, temp_transf2;
    float temp_comp, temp_transf;

    // Iniciamos el MPI.
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Inicializamos las variables donde van las matrices.
    data1 = (int *) malloc(M * N * sizeof(int));
    data2 = (int *) malloc(M * N * sizeof(int));
    result = (int *) malloc(M * sizeof(int));

    // Inicializamos las matrices en el proceso 0.
    if (rank == 0) {
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                data1[i * N + j] = (i + j) % 5;
                data2[i * N + j] = 4 - data1[i * N + j];
            }
        }
    }

    // Mandamos a cada proceso el array result y medimos su tiempo de transferencia con gettimeofday.
    gettimeofday(&temp_transf1, NULL);
    MPI_Bcast(result, N, MPI_INT, 0, MPI_COMM_WORLD);
    gettimeofday(&temp_transf2, NULL);

    // Lo guardamos en la variable temp_transf.
    temp_transf = (temp_transf2.tv_usec - temp_transf1.tv_usec) + 1000000 * (temp_transf2.tv_sec - temp_transf1.tv_sec);

    // Calculamos el numero de rows que va a cada proceso.
    int rows = N / numprocs;
    // Si es el último miramos si la division no es entrea y le sumamos si hay una parte que sobra.
    if (rank == (numprocs - 1)) {
        rows = rows + (N % numprocs);
    }

    // Reservamos memoria para las matrices locales de cada proceso, estas van a recibir los trozos de las matrices.
    local_data1 = malloc(N * rows * sizeof(int));
    local_data2 = malloc(N * rows * sizeof(int));
    local_result = malloc(N * sizeof(int));

    // Si el proceso es el 0, creamos sentcounts y dipl para la la función MPI_Scatterv ( no tengo ni idea de que es esto xd).
    if (rank == 0) {
        tam = malloc(sizeof(int) * numprocs);
        despl = malloc(sizeof(int) * numprocs);
        for (i = 0; i < numprocs; i++) {
            if (i == numprocs - 1) {
                tam[i] = N * (rows + (N % numprocs));
            } else {
                tam[i] = rows * N;
            }
        }
        despl[0] = 0;
        for (i = 1; i < numprocs; i++){
            despl[i] = despl[i - 1] + tam[i - 1];
        }
    }

    // Mandamos los trozos de las matrices originales y medimos su tiempo y lo sumamos.
    gettimeofday(&temp_transf1, NULL);
    MPI_Scatterv(data1, tam, despl, MPI_INT, local_data1, rows * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(data2, tam, despl, MPI_INT, local_data2, rows * N, MPI_INT, 0, MPI_COMM_WORLD);
    gettimeofday(&temp_transf2, NULL);

    temp_transf += (temp_transf2.tv_usec - temp_transf1.tv_usec)+ 1000000
                                                                    * (temp_transf2.tv_sec - temp_transf1.tv_sec);


    // Parte computacional, donde hacemos el resultado de cada trozo de cadena y medimos su tiempo.
    gettimeofday(&temp_comp1, NULL);
    for (i = 0; i < rows; i++) {
        local_result[i] = 0;
        for (j = 0; j < N; j++) {
            local_result[i] += base_distance(local_data1[i], local_data2[i]);
        }
    }
    gettimeofday(&temp_comp2, NULL);

    temp_transf += (temp_transf2.tv_usec - temp_transf1.tv_usec) + 1000000 * (temp_comp2.tv_sec - temp_comp1.tv_sec);

    // Creamos sentcounts y displ para MPI_Gatherv como para MPI_Scatterv.
    if (rank == 0) {
        for (i = 0; i < numprocs; i++) {
            tam[i] = tam[i] / N;
        }
        for (i = 1; i < numprocs; i++) {
            despl[i] = despl[i - 1] + tam[i - 1];
        }
    }

    // Enviamos las soluciones al proceso 0 y medimos el tiempo que tardan en recibirse y se suman.
    gettimeofday(&temp_transf1, NULL);
    MPI_Gatherv(local_result, rows, MPI_INT, result, tam, despl, MPI_INT, 0, MPI_COMM_WORLD);
    gettimeofday(&temp_transf2, NULL);

    temp_transf +=(temp_transf2.tv_usec - temp_transf1.tv_usec) + 1000000 * (temp_transf2.tv_sec - temp_transf1.tv_sec);

    temp_comp = (temp_comp2.tv_usec - temp_comp1.tv_usec) + 1000000 * (temp_comp2.tv_sec - temp_comp1.tv_sec);

    /*Display result */
    if (DEBUG) {
        if (rank == 0) {
            for (i = 0; i < M; i++) {
                printf(" %d \t ", result[i]);
            }
        }
    } else {
        printf("Proceso %d:\t Tiempo Computacional = %lf\t Tiempo transferencia = %lf\n",
               rank, (double) temp_comp / 1E6, (double) temp_transf / 1E6);
    }

    // Se libera memoria y se cierra el MPI.
    free(data1);
    free(data2);
    free(local_result);
    free(result);
    MPI_Finalize();

    return 0;
}


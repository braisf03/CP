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

/*
Dividid las matrices entre p procesos, cada uno con rows = M/p filas
(por simplicidad, empezad con el caso en que consideraremos que el
n ́umero de procesos es m ́ultiplo de M: M mod p = 0). VAMOS ASI PRIMERO

Cada tarea se encargar ́a de calcular M/p elementos del vector resultado. ESTO SE HACE EN ROWS

En la entrega considerad cualquier n ́umero de procesos, no solo m ́ultiplos
de M. ESTO AUN NO QUE ME PIERDO

La inicializaci ́on de la matriz la hace el proceso 0. ESTO SE HACE

Distribuir datos a todos los procesos con operaciones colectivas.

Recolecci ́on del vector resultado con operaciones colectivas.

La E/S (printf) la hace el proceso 0.

Imprimid por separado tiempo de comunicaciones y tiempo computaci ́on
de cada proceso.

*/

int main(int argc, char *argv[]) {

    int i, j, numprocs, rank, rows;
    int *data1, *data2;
    int *result;
    struct timeval tc1, tc2, tt1, tt2;
    long comp_time, trans_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    rows = M / numprocs;
    if (M%numprocs!=0) {
        rows++;
    }

    if (rank == 0) {
        data1 = (int *) malloc(M * N * sizeof(int));
        data2 = (int *) malloc(M * N * sizeof(int));
        result = (int *) malloc(rows* N * sizeof(int));
        /* Initialize Matrices */
        for (i = 0; i < M; i++) {
            for (j = 0; j < N; j++) {
                data1[i * N + j] = (i + j) % 5;
                data2[i * N + j] = 4 - data1[i * N + j];
            }
        }
    }

    int *local_data1 = malloc(rows * N * sizeof(int));
    int *local_data2 = malloc(rows * N * sizeof(int));
    int *local_result = malloc(rows * sizeof(int));

    gettimeofday(&tt1, NULL);
    MPI_Scatter(data1, rows * N, MPI_INT, local_data1, rows * N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(data2, rows * N, MPI_INT, local_data2, rows * N, MPI_INT, 0, MPI_COMM_WORLD);
    gettimeofday(&tt2, NULL);

    trans_time = (tt2.tv_usec - tt1.tv_usec) + 1000000 * (tt2.tv_sec - tt1.tv_sec);

    gettimeofday(&tc1, NULL);
    for (i = 0; i < rows - 1; i++) {
        local_result[i] = 0;
        for (j = 0; j < N; j++) {
            local_result[i] += base_distance(local_data1[i * N + j], local_data2[i * N + j]);
        }
    }

    gettimeofday(&tc2, NULL);

    comp_time = (tc2.tv_usec - tc1.tv_usec) + 1000000 * (tc2.tv_sec - tc1.tv_sec);

    gettimeofday(&tt1, NULL);
    MPI_Gather(local_result, rows, MPI_INT, result, rows, MPI_INT, 0, MPI_COMM_WORLD);
    gettimeofday(&tt2, NULL);

    trans_time += (tt2.tv_usec - tt1.tv_usec) + 1000000 * (tt2.tv_sec - tt1.tv_sec);

    /*Display result */
    if (DEBUG) {
        if (rank == 0) {
            for (i = 0; i < M; i++) {
                printf(" %d \t ", result[i]);
            }
        }
    } else {
        printf("Process(%d):\t Comp_Time: %lf (seconds)\t Trans_Time %lf (seconds) \n", rank, (double) comp_time / 1E6,
               (double) trans_time / 1E6);
    }

    if(rank==0){
        free(data1);
        free(data2);
        free(result);
    }

    free(local_data1);
    free(local_data2);

    MPI_Finalize();

    return 0;
}

## Práctica 1 ##
En esta práctica se pide realizar un implementación SPMD de un programa que cuenta cuantas letras hay
en una cadena en particular.

# Enunciado #
Para ello se deben usar las funciones MPI (Message Passing Interface), en las que el proceso 0 debe 
hacer la entrada/salida (scanf/printf) y debe distribuir *n* y *L* al resto de procesos (con MPI_Send).

Nota: *n* es la longitud de la cadena y *L* la letra ser buscada.

La carga de trabajo se debe repartir con un bucle for con "paso" i+=numprocs en vez de i++.

Para terminar el proceso 0 debe recoger el número de aparicioenes detectada en cada proceso (con MPI_Recv).

Nota: MPI_Send y MPI_Recv no separan como deben la cadena a separar, por lo tanto es probable que si
tienes un número de procesos que no divida (en números enteros ) a la cadena puede dar problemas.

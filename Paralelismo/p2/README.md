# Práctica 2 #
En esta práctica se pide realizar una extensión de la práctica 1 hecha anteriormente.

El main.c manda la letra, la longitud de la cadena y la cadena a distintos procesos que con el
paso i=+numprocs barren la cadena global para ver la cantidad de veces que aparece la letra en cuestión.

## Enunciado ##
Para ello se deben usar las funciones MPI (Message Passing Interface), en las que el proceso 0 debe 
hacer la entrada/salida (argv/printf) y debe distribuir *n* y *L* al resto de procesos (con MPI_Send).

Nota: *n* es la longitud de la cadena y *L* la letra ser buscada. Ahora con operación colectiva MPI! `#f03c15`

La carga de trabajo se debe repartir con un bucle for con "paso" i+=numprocs en vez de i++.

Para terminar el proceso 0 debe recoger el número de aparicioenes detectada en cada proceso (con MPI_Recv).

Nota: MPI_Send y MPI_Recv no separan como deben la cadena, por lo tanto es probable que si
tienes un número de procesos que no divida (en números enteros ) a la cadena puede dar problemas.
- ![#f03c15](https://placehold.co/15x15/f03c15/f03c15.png) `#f03c15`

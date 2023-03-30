# Práctica 3 #
En esta práctica se va a implementar un programa en paralelo que reciba dos cadenas de ADN y compruebe la similaridad entre ellas.

## Enunciado ##
Dividid las matrices entre p procesos, cada uno con rows = M/p filas
(por simplicidad, empezad con el caso en que consideraremos que el
número de procesos es múltiplo de M: M mod p = 0).

Cada tarea se encargará de calcular M/p elementos del vector resultado.

En la entrega considerad cualquier número de procesos, no solo múltiplos de M.

Implementación SPMD.

La inicialización de la matriz la hace el proceso 0.

Distribuir datos a todos los procesos con operaciones colectivas.

Recolección del vector resultado con operaciones colectivas.

La E/S (printf) la hace el proceso 0.

Imprimid por separado tiempo de comunicaciones y tiempo computación de cada proceso.

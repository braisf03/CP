# Práctica 3 #
En esta práctica se va a implementar un programa en paralelo que reciba dos cadenas de ADN y compruebe la similaridad entre ellas.

## Enunciado ##
Dividid las matrices entre p procesos, cada uno con rows = M/p filas
(por simplicidad, empezad con el caso en que consideraremos que el
n ́umero de procesos es m ́ultiplo de M: M mod p = 0).

Cada tarea se encargar ́a de calcular M/p elementos del vector resultado.

En la entrega considerad cualquier n ́umero de procesos, no solo m ́ultiplos
de M.

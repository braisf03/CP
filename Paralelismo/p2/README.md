# Práctica 2 #
En esta práctica se pide realizar una extensión de la práctica 1 hecha anteriormente.

El main.c manda la letra, la longitud de la cadena y la cadena a distintos procesos que con el
paso i=+numprocs barren la cadena global para ver la cantidad de veces que aparece la letra en cuestión.

## Enunciado (Parte 1) ##
Para ello se deben usar las funciones MPI (Message Passing Interface), en las que el proceso 0 debe 
hacer la entrada/salida (scanf/printf) y debe distribuir *n* y *L* al resto de procesos (con MPI_Send).
**Ahora con operación colectiva MPI!**

Nota: *n* es la longitud de la cadena y *L* la letra ser buscada.

La carga de trabajo se debe repartir con un bucle for con "paso" i+=numprocs en vez de i++.

Para terminar el proceso 0 debe recoger el número de aparicioenes detectada en cada proceso (con MPI_Recv).
**Ahora con operación colectiva MPI!**

Nota: MPI_Send y MPI_Recv no separan como deben la cadena, por lo tanto es probable que si
tienes un número de procesos que no divida (en números enteros ) a la cadena puede dar problemas.

## Enunciado (Parte 2) ##

Se pide implementar una función colectiva en árbol binomial, implementación que
denominaremos MPI BinomialColectiva, que utilizaremos *SOLO* en la distribución de n y L.

Implementación de Bcast con árbol binomial (MPI BinomialBcast):Mismos parámetros que MPI Bcast 
(consultar p ́agina man de MPI Bcastpara obtener cabecera), asumiendo por simplicidad que el root es el 0.
En el paso “i” los procesos con myrank < 2⁽ᶦ⁻¹⁾ se comunican con el proceso myrank + 2⁽ᶦ⁻¹⁾.




Posteriormente introducci ́on de implementaci ́on propia de colectiva
SOLO para la recolecci ́on de count, inicialmente utilizando las
mismas operaciones de Send/Recv que en la implementaci ́on sin
colectivas (bucle for de Recv), implementaci ́on que denominaremos
MPI FlattreeColectiva. Asumir que la operaci ́on a realizar ser ́a una
suma. El resto de par ́ametros de la cabecera deben ser los mismos
que los de la colectiva est ́andar de MPI (incluido controlar el error).

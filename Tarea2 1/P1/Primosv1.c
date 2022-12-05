#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define BLOCK_LOW(id,p,n) ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n)(BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n)(BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)

int main (int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int id, i, j, flag, np, n,valoracion, total, inicio, fin, num_elems_rango,root = 0;

  // Para calcular los tiempos
  double t1,t2;

  MPI_Comm_size(MPI_COMM_WORLD, &np); // Numero total de procesos
  MPI_Comm_rank(MPI_COMM_WORLD, &id); // Valor de nuestro identificador
    
    n = atoi(argv[1]);
    inicio = BLOCK_LOW(id,np,n)+1; // para que empiece en 1
    fin    = BLOCK_HIGH(id,np,n)+1;
    num_elems_rango = BLOCK_SIZE(id,np,n);

    MPI_Barrier(MPI_COMM_WORLD);  // sincronización
    t1 = MPI_Wtime();

    if(id == 0){
        printf("Programa que encuentra el numero de primos menores o iguales a %d\n",n);
    }

    valoracion = 0;
    for (i = inicio; i <= fin; i++){
        flag = 0;
        for (j = 2; j <= i/2; j++){
            if(i % j == 0){
              flag = 1;
                break;
            }
        }
        if (flag == 0 && i != 1){
            //printf("%d es un numero primo\n",i);
            valoracion++;
        }
    }

    MPI_Send(&valoracion, 1, MPI_INT, root, 9, MPI_COMM_WORLD);

    total = 0;
    if(id == root){
        for(i = 0 ; i < np ; i++){
            MPI_Recv(&valoracion, 1, MPI_INT, i, 9, MPI_COMM_WORLD, MPI_STATUS_IGNORE);     
            total += valoracion;     
        }
        printf("\nNumeros primos totales: %d\n", total);
    } 

    MPI_Barrier(MPI_COMM_WORLD);  // sincronización
    t2 = MPI_Wtime();

    if(id == 0){
	printf("Numero de procesos: %d, Tiempo ejecutado en: %.10f \n",np, t2-t1);
	//printf("\nTiempo total al calcular la cantidad de numeros primos fue: %.10f segundos \n", t2-t1);
    }
    
    MPI_Finalize(); 
}

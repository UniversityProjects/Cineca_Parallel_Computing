#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
int my_rank, numprocs;
char message[100];
int dest, tag, source;
MPI_Status status;
MPI_Request request;

tag = 0;

MPI_Init (&argc, &argv);
MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
MPI_Comm_size (MPI_COMM_WORLD, &numprocs);

int A = my_rank;
int sum = 0;
int tmp = 0;
int i = 0;

if (my_rank == 0) {	
	
	dest = 1;
	source = numprocs-1;

} else if (my_rank == numprocs-1) {
	
	dest = 0;
	source = numprocs - 1;
	
} else {
	
	dest = my_rank + 1;
	source = my_rank - 1;
	
}

for (i = 0; i < numprocs; i++) {
	
	printf("Rank %d - Dest %d - Source %d ", my_rank, dest, source);
	MPI_Isend ( &A, 1, MPI_INTEGER, dest, tag, MPI_COMM_WORLD, &request);
	printf (" message sent: %d ", A);
	MPI_Irecv ( &tmp, 1, MPI_INTEGER, source, tag, MPI_COMM_WORLD, &request);	
	printf (" -  message received: %d \n", tmp);
	sum += tmp;
	A = sum;
	
}

	
printf("Rank %d: Sum = %d \n", my_rank, sum);

MPI_Finalize();

return 0;

}

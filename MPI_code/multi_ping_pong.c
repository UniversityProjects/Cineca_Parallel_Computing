#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
	
int my_rank, numprocs;
char message[100];
int dest, tag, source;
MPI_Status status;

tag = 0;

MPI_Init (&argc, &argv);
MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
MPI_Comm_size (MPI_COMM_WORLD, &numprocs);



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
	
	
sprintf (message, "%d", my_rank);

tag = 0;
	
MPI_Send (message, sizeof(message), MPI_CHAR, dest, tag, MPI_COMM_WORLD);	
printf("Rank %d: Message Sent\n", my_rank);
	
MPI_Recv (message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
printf("Rank %d: Message Received from: %s \n", my_rank, message);

MPI_Finalize();

return 0;

}

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

printf("\n");
printf("I am the process %d \n", my_rank);

if (my_rank != 0) {
	
	printf("Rank %d: Inside if\n", my_rank);

	sprintf (message, "Greetings from process %d ! \0", my_rank);
	dest = 0;
	tag = 0;
	MPI_Send (message, sizeof(message), MPI_CHAR, dest, tag, MPI_COMM_WORLD);
	printf("Rank %d: Message Sent\n", my_rank);

} else {
	
	printf("Rank %d: Inside else\n", my_rank);
	
	for (source = 1; source <= (numprocs - 1); source++) {
		
		printf("Rank %d: Inside for - source = %d ", my_rank, source);

		MPI_Recv (message, 100, MPI_CHAR, source, tag, MPI_COMM_WORLD, &status);
		
		printf("Message received: \n");
		printf ("%s \n\n", message);

	}
	
}

MPI_Finalize();

return 0;

}

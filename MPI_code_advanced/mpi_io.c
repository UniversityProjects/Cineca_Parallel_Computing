#include <stdio.h>
#include <mpi.h>

#define DIM_BUF 10

// Open written file with the command:
// od -i output.dat\@

int main(int argc, char **argv){
	
   int myrank, nproc;
   int buf[DIM_BUF];
   int i;
   char file_name[10] = {'o', 'u', 't', 'p', 'u', 't', '.', 'd', 'a', 't'};
   MPI_File fh;
   MPI_Status status;
   MPI_Info info;


   MPI_Init(&argc, &argv);
   MPI_Comm_size(MPI_COMM_WORLD, &nproc);
   MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   
   

   for(i = 0; i < DIM_BUF; i++) {
	   
	   buf[i] = myrank*DIM_BUF+i;

	}

/* Open the file and write  */
	MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_WRONLY + MPI_MODE_CREATE, MPI_INFO_NULL
, &fh);

	MPI_File_write_ordered (fh, buf, DIM_BUF, MPI_INT, &status);

	MPI_File_close(&fh);


/* Re-open the file and read data */

	MPI_File_open(MPI_COMM_WORLD, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);

	MPI_File_read_ordered (fh, buf, DIM_BUF, MPI_INT, &status);

	MPI_File_close(&fh);

   printf("myid %d, buffer after read:",myrank);
   
   for (i = 0; i < DIM_BUF; i++) {
	   
	   printf("%d ",buf[i]);
   
   }
   
   printf("\n\n");

   MPI_Finalize();
   
   return 0;

}

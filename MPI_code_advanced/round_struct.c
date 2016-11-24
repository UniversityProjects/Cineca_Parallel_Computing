/*
   Round_pack 
   
   Example of communicating heterogeneous data using MPI_Struct   
  
*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


MPI_Datatype mpi_type;  /*  New MPI data type  */

typedef struct {   /* data structure */
   int a, b;
   double v[3];
   char str[9];
} heterogeneous;

#define buffer_size 42
char buffer[buffer_size];

MPI_Status v_status;

int proc_rank, mpi_procs=0;
char processor_name[MPI_MAX_PROCESSOR_NAME];
MPI_Comm mpi_world;

int start_mpi(int argc, char **argv)
{
   int err, namelen;
   
   if ( mpi_procs > 0 ) return(0); /* MPI is already initialized */
   if ( (err = MPI_Init(&argc, &argv)) != MPI_SUCCESS ) return(err);
   mpi_world = MPI_COMM_WORLD;
   if ( (err = MPI_Comm_rank(mpi_world, &proc_rank)) != MPI_SUCCESS ) return(err);
   if ( (err = MPI_Comm_size(mpi_world, &mpi_procs)) != MPI_SUCCESS ) return(err);
   if ( (err = MPI_Get_processor_name(processor_name,&namelen)) != MPI_SUCCESS ) return(err);
   if ( namelen >= 8 ) processor_name[8]='\0';
   return(0);
}


void initialize_data(heterogeneous *given)
{
   int i;

   given->a = proc_rank;
   given->b = 0;
   for ( i=0; i<3; i++ ) given->v[i] = i;
   strcpy(given->str,processor_name);
   printf(" Processor %d initialized: %d, %d, %lf, %lf, %lf, %s\n",proc_rank,
         given->a,given->b,given->v[0],given->v[1],given->v[2],given->str);

}


void update_data(heterogeneous *given, heterogeneous *taken)
{
   int i;

   printf(" Processor %d received: %d, %d, %lf, %lf, %lf, %s\n",proc_rank,
         taken->a,taken->b,taken->v[0],taken->v[1],taken->v[2],taken->str);
   given->a = proc_rank;
   given->b = taken->b + 1;
   for ( i=0; i<3; i++ ) given->v[i] = taken->v[i] + i;
   strcpy(given->str,processor_name);
}



/*
   Example program: round robin 
*/
  
int main(int argc, char **argv)
{
   int i, length;
   int next_proc, msg_tag = 111, prec_proc;
   heterogeneous given, taken;

   if ( start_mpi(argc, argv) != 0 ) {
      fprintf(stderr,"Error start_mpi\n");
   }

   next_proc = ( proc_rank+1 ) % mpi_procs;
   prec_proc = (mpi_procs+proc_rank-1) % mpi_procs;

   initialize_data(&given);
   
   // Packing Structure
	int num_blk = 4;
	int v_len_blk[4] = {1, 1, 3, 9};
	MPI_Aint v_head[4] = {0, 4, 8, 32};
	MPI_Datatype v_el_type[4] = {MPI_INT, MPI_INT, MPI_DOUBLE, MPI_CHAR};

	MPI_Type_struct(num_blk, v_len_blk, v_head, v_el_type, &mpi_type);
	MPI_Type_commit(&mpi_type);

   if ( proc_rank == 0 ) {
            
      MPI_Send (&given, 1, mpi_type, next_proc, msg_tag, mpi_world);

   }
   
   MPI_Recv (&taken, 1, mpi_type, prec_proc, msg_tag, mpi_world, &v_status);
   
    if ( proc_rank == 0 ) {
      printf(" Processor %d received: %d, %d, %lf, %lf, %lf, %s\n",proc_rank,
         taken.a,taken.b,taken.v[0],taken.v[1],taken.v[2],taken.str);
   } else  {
      
      update_data(&given,&taken);
               
      MPI_Send (&given, 1, mpi_type, next_proc, msg_tag, mpi_world);
      
   } 

   return(MPI_Finalize());
   
}

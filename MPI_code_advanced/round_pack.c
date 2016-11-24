/*
   Round_pack 
   
   Example of communicating heterogeneous data using MPI_Pack/Unpack    
  
*/
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


int pack_data(int* lbuffer, char* buffer, int sbuffer, heterogeneous* data)
{
   int err;
   
   int pos = 0;
   
   err = MPI_Pack(&data->a, 1, MPI_INT, buffer, sbuffer, lbuffer, mpi_world);
   err = MPI_Pack(&data->b, 1, MPI_INT, buffer, sbuffer, lbuffer, mpi_world);
   err = MPI_Pack(&data->v, 3, MPI_DOUBLE, buffer, sbuffer, lbuffer, mpi_world);
   err = MPI_Pack(&data->str, 9, MPI_CHAR, buffer, sbuffer, lbuffer, mpi_world);

   
   return(0);
}


int unpack_data(int* lbuffer, char* buffer, int sbuffer, heterogeneous* data)
{
   int err;
   
	int pos = 0;
   
   err = MPI_Unpack(buffer, sbuffer, lbuffer, &data->a, 1, MPI_INT, mpi_world);
   err = MPI_Unpack(buffer, sbuffer, lbuffer, &data->b, 1, MPI_INT, mpi_world);
   err = MPI_Unpack(buffer, sbuffer, lbuffer, &data->v, 3, MPI_DOUBLE, mpi_world);
   err = MPI_Unpack(buffer, sbuffer, lbuffer, &data->str, 9, MPI_CHAR, mpi_world);
   
   return(0);
}

#include <string.h>

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

   if ( proc_rank== 0 ) {
      length = 0;
      if ( pack_data(&length, buffer, buffer_size, &given) != 0 )  {
         fprintf(stderr,"Error pack_data\n");
      }
      
      MPI_Send (buffer, length, MPI_PACKED, next_proc, msg_tag, mpi_world);

   }
   
   length=41;
   MPI_Recv (buffer, length, MPI_PACKED, prec_proc, msg_tag, mpi_world, &v_status);

   length = 0; 
   if ( unpack_data(&length, buffer, buffer_size, &taken) != 0 )  {
         fprintf(stderr,"Error unpack_data\n");
   }
   if ( proc_rank == 0 ) {
      printf(" Processor %d received: %d, %d, %lf, %lf, %lf, %s\n",proc_rank,
         taken.a,taken.b,taken.v[0],taken.v[1],taken.v[2],taken.str);
   } else  {
      update_data(&given,&taken);
      length = 0;
      if ( pack_data(&length, buffer, buffer_size, &given) != 0 )  {
         fprintf(stderr,"Error pack_data\n");
      }      
      
       MPI_Send (buffer, length, MPI_PACKED, next_proc, msg_tag, mpi_world);
      
   } 
   return(MPI_Finalize());
}

#ifdef _OPENMP
	#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {
	
	#pragma omp parallel 
	{
		
	#ifdef _OPENMP
		
		int thread_id = omp_get_thread_num();
		int thread_num = omp_get_num_threads();

	#endif

	
		fprintf (stdout, "I am thread %d of %d \n", thread_id, thread_num);

	}

	return(0);

}

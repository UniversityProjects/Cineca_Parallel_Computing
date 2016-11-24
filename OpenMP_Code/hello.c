#ifdef _OPENMP
	#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

	#pragma omp parallel 
	{
	
		fprintf (stdout, " Hello world!\n");

	}

	return(0);

}

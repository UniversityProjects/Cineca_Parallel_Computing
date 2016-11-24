#ifdef _OPENMP
	#include <omp.h>
#endif
#include <stdio.h>


double f ( double );

double f ( double a ) {
    
    return (4.0 / (1.0 + a*a));

}

int main (int argc, char *argv[]) {
	
	int n = 1; // Number Of Intervals
	int a = 0; // Left Interval Extremum
	int b = 1; // Right Interval Extremum
	int i = 0; // For Loop Variable
	double PI25DT = 3.141592653589793238462643; // Control Result
	double startwtime = 0.0, endwtime; // Start And End Times
	double pi; // Final Result
	double h; // Step Size
	
	while (n != 0) {
	
		printf("Enter the number of intervals: (0 quits) \n");
        scanf("%d",&n);
        
        pi = 0.0;
        
        if (n == 0) { // Quit Program
			
			return 0;
			
		} else {
			
			// Step Size Assignment
			h = 1.0 / ((double) n);
	
			#pragma omp parallel for reduction(+:pi)
			for (i = 1; i < n; i++) {
			
				pi += h * f (h * (i - 0.5) );
				
			} // end parallel for
		
			// Result Output
			printf("pi is approximately %.16f, Error is %.16f\n",
                       pi, fabs(pi - PI25DT));
                 
            // End Time Computation  
            #ifdef _OPENMP
				endwtime = omp_get_wtime();
				printf("wall clock time = %f\n", endwtime-startwtime);
			#endif   
		
		} // end if
			
	} // end while

	return(0);

}

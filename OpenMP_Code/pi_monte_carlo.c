#ifdef _OPENMP
	#include <omp.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


double f ( double );

double f ( double a ) {
    
    return (4.0 / (1.0 + a*a));

}

int main (int argc, char *argv[]) {
	
	int n = 1; // Number Of Intervals
	double x;
	double y;
	double x1 = -1.0;
    double x2 = 1.0;
    double y1 = -1.0;
    double y2 = 1.0;
    double SqPoints  = 0;
    double CiPoints  = 0;
	int i = 0; // For Loop Variable
	double PI25DT = 3.141592653589793238462643; // Control Result
	double startwtime = 0.0, endwtime; // Start And End Times
	double pi; // Final Result
	
	while (n != 0) {
	
		printf("Enter the number of intervals: (0 quits) \n");
        scanf("%d",&n);
        
        if (n == 0) { // Quit Program
			
			return 0;
			
		} else {
			
			pi = 0.0;
			x1 = -1.0;
            x2 = 1.0;
            y1 = -1.0;
            y2 = 1.0;
            SqPoints  = 0;
            CiPoints  = 0;

			#pragma omp parallel reduction(+:pi)
			{
				#pragma omp for
				for (i = 1; i < n; i++) {
			
					x = rand(); 
					x = x / RAND_MAX; 
					x = x1 + x * (x2 - x1);
					
					y = rand(); 
					y = y / RAND_MAX; 
					y = y1 + y * (y2 - y1);
                
					SqPoints++;
					
					if ( sqrt( x*x + y*y ) <= 1.0 ) {
						
						CiPoints++;
						
					} // end if
				
				} // end for	 
				
				//pi = 4.0 * (double)CiPoints / (double)SqPoints;
				pi = (double)CiPoints / (double)SqPoints;
				
			} // end parallel
		
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

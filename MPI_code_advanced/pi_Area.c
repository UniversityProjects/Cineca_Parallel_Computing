#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <mpi.h>

/*
!   pi_Area.c - compute Pi by counting random points in a circle 
!                  circumscribed in a square.
!                The circle has centre in the origin and radius 1
!
!
!   Each process:
!    1) receives the number of points to be generated
!    2) generates and counts points in rectangle and circle
!    3) Synchronizes for a global summation.
!   Process 0 prints the result.
!
!  Variables:
!
!    pi          the calculated result
!    n           number of random points 
!    x1, x2      extremes of each rectangle
!    SqPoints    points in each rectangle
!    CiPoints    points in circle      
!
!*/

int main( int argc, char *argv[])
{
    int done = 0, n, myid, numprocs, i, err;
    double PI25DT = 3.141592653589793238462643;
    double mypi, pi, h;
    double x, y, x1, x2, y1, y2;
    int SqPoints, CiPoints, my_SqPoints, my_CiPoints;
    double startwtime = 0.0, endwtime;
    int  namelen;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocs);
    MPI_Comm_rank(MPI_COMM_WORLD,&myid);
    MPI_Get_processor_name(processor_name,&namelen);

    fprintf(stderr,"Process %d on %s\n",
            myid, processor_name);

    n = 0;
    while (!done)
    {
        if (myid == 0)
        {
            printf("Enter the number of points: (0 quits) \n");
            scanf("%d",&n);

            startwtime = MPI_Wtime();
        }
        
        MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        if (n <= 0 || err != 0 )
            done = 1;
        else
        {
            h   = 2.0 / (double) numprocs;
            x1 = -1.0 + myid * h;
            x2 = x1 + h;
            y1 = -1.0;
            y2 = 1.0;
            my_SqPoints  = 0;
            my_CiPoints  = 0;

            for (i = 1; i <= n; i += numprocs)
            {
                x = rand(); x = x / RAND_MAX; x = x1 + x * (x2 - x1);
                y = rand(); y = y / RAND_MAX; y = y1 + y * (y2 - y1);
                my_SqPoints++;
                
                if ( ( x*x + y*y ) <= 1.0 ) {
					
					my_CiPoints++;
					
				}
				
            }

            MPI_Reduce(&my_SqPoints, &SqPoints, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
            MPI_Reduce(&my_CiPoints, &CiPoints, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

            if (myid == 0)
            {
				
                pi = 4.0 * (double)CiPoints / (double)SqPoints;
                printf("Pi is approximately %.16f, Error is %.16f\n",
                       pi, fabs(pi - PI25DT));
                endwtime = MPI_Wtime();
                printf("wall clock time = %f\n",
                       endwtime-startwtime);
            }
        }
    }
   
   
	MPI_Finalize();

    return 0;
}


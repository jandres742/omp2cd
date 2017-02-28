#include "lastprivate.h"
int main(int argc, char** argv)
{
    int N = 10;
    if (argc > 1)
        N = atoi(argv[1]);

    MYTYPE* A = (double*)malloc(N * sizeof(double));

    int i;
    for (i = 0; i < N; i++) {
        *(A + i) = i + 1;
    }

    i = 0;

    int w = 20;

#pragma omp parallel
	{
#pragma omp for firstprivate(w) lastprivate(i) 
		for (i = 0; i < N; i++){
			if(w != 20)
				fprintf(stderr, "Error in thread = %d for firstprivate, w = %d, should be %d\n",
					omp_get_thread_num(), w, 20);
				
			*(A + i) += *(A + i)  + *(A + i) + w;
		}

#pragma omp barrier
		
		if(i != N)
			fprintf(stderr, "Error in thread = %d for lastprivate, i = %d, should be %d\n",
				omp_get_thread_num(), i, N);
		
	}


    return 0;
}

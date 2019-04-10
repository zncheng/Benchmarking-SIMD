#include <stdio.h>
#include <immintrin.h>
#include <sys/time.h>
#include <unistd.h>

double a[24] = {0.0};
double b[24] = {0.0};

double baseline(const double *a, const double *b){
	//double ret = 0;
	//for (int i  = 0; i < 24; i++){
	//	
	//}
	return 0;
}


int main()
{

	//double vec[24] = {}
	struct timeval start_time, end_time;

	gettimeofday(&start_time, NULL); 
	__m256i hello;
	__m256d a = _mm256_set_pd(2.3, 2.0, 3.0, 4.0);
	__m256d b = a;

	union{__m256d c; double output[4];};

	c = _mm256_add_pd(a, b);

	//__m256d c = _mm256_add_pd(a, b);
	//__attribute__ ((aligned (64))) double output[4];
	//_mm256_store_pd(output, c);

	printf("%f %f %f %f\n", output[0], output[1], output[2], output[3]);
	printf("%f %f %f %f\n", ((double*)&c)[0], ((double*)&c)[1], ((double*)&c)[2], ((double*)&c)[3]);
	return 0;
}

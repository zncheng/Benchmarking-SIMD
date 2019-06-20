#include <stdio.h>
#include <immintrin.h>
#include <sys/time.h>
#include <unistd.h>

#define attr_num 100

double add_baseline(const double *a, const double *b){
	double c[attr_num] = {0.0};
	for (int i  = 0; i < attr_num; i++){
		c[i] = a[i] + b[i];
	}
	return c[attr_num - 1];
}

double addmul_baseline(const double *a, const double *b){
	double c[attr_num] = {0.0};
	for (int i  = 0; i < attr_num; i++){
		c[i] = a[i] * (i * 1.0) + b[i];
	}
	return c[attr_num - 1];
}

double add_avx(const double *a, const double *b){
	__attribute__ ((aligned (32))) double c[attr_num] = {0.0};
	for (int i = 0; i < attr_num / 4; i ++){
		__m256d x = _mm256_load_pd(a + 4 * i);
		__m256d y = _mm256_load_pd(b + 4 * i);
		__m256d ret = _mm256_add_pd(x, y);
		_mm256_store_pd(c + 4 * i, ret);
	}
	for (int i = attr_num - attr_num % 4; i < attr_num; i ++){
		c[i] = a[i] + b[i];
	} 
	return c[attr_num -1];
}

double addmul_avx(const double *a, const double *b){
	__attribute__ ((aligned (32))) double c[attr_num] = {0.0};
	for (int i = 0; i < attr_num / 4; i ++){
		__m256d x = _mm256_load_pd(a + 4 * i);
		__m256d y = _mm256_set_pd(3.0+i*4, 3.0+i*4, 1.0+i*4, 0.0+i*4);
		__m256d z = _mm256_mul_pd(x, y); 
		__m256d t = _mm256_load_pd(b + 4 *i);
		__m256d ret =  _mm256_add_pd(z, t);
		_mm256_store_pd(c + 4 * i, ret);
	}
	for (int i = attr_num - attr_num % 4; i < attr_num; i ++){
		c[i] = a[i] * i + b[i];
	} 
	return c[attr_num -1];
}

double addmul_fma(const double *a, const double *b){
	__attribute__ ((aligned (32))) double c[attr_num] ={0.0};
	for (int i = 0 ; i < attr_num /4; i ++){
		__m256d x = _mm256_load_pd(a + 4 * i);
		__m256d y = _mm256_set_pd(3.0+i*4, 2.0+i*4, 1.0+i*4, 0.0+i*4);
		__m256d z = _mm256_load_pd(b + 4 *i);
		__m256d ret = _mm256_fmadd_pd(x, y, z); //x*y+z
		_mm256_store_pd(c + 4 * i, ret);
	}
	for (int i = attr_num - attr_num % 4; i < attr_num; i ++){
		c[i] = a[i] *i + b[i];
	} 
	return c[attr_num -1];
}

int main()
{
	__attribute__ ((aligned (32))) double a[attr_num] = {0.0};
	__attribute__ ((aligned (32))) double b[attr_num] = {0.0};
	double base_time = 0;
	double used_time = 0;

	for (int i = 0 ; i < attr_num; i++){
		a[i] = 1.0 / i;
		b[i] = 2.0 / (i +1);
	}
	struct timeval start_time, end_time;

	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 100000000; i ++){
		add_baseline(a, b);
		//printf("add result %lf\n", add_baseline(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	base_time = used_time;
	printf("add baseline used time: %lf \n", used_time);

	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 100000000; i ++){
		add_avx(a, b);
		//printf("add_avx result %lf\n", add_avx(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	printf("add_avx used time: %lf speedup %lf \n", used_time, base_time / used_time);

	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 100000000; i ++){
		addmul_baseline(a, b);
		//printf("addmul_baseline result %lf\n", addmul_baseline(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	base_time = used_time;
	printf("addmul baseline used time: %lf \n", used_time);
	
	//gettimeofday(&start_time, NULL); 
	//for (int i = 1; i <= 100000000; i ++){
	//	addmul_avx(a, b);
	//	//printf("addmul_avx result %lf\n", addmul_avx(a, b));
	//}
	//gettimeofday(&end_time, NULL);
	//used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	//printf("addmul_avx used time: %lf speedup %lf \n", used_time, base_time / used_time);
	
	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 100000000; i ++){
		addmul_fma(a, b);
		//printf("addmul_fma result %lf\n", addmul_fma(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	printf("addmul_fma used time: %lf speedup %lf \n", used_time, base_time / used_time);

	return 0;
}

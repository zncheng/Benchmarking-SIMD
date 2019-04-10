#include <stdio.h>
#include <immintrin.h>
#include <sys/time.h>
#include <unistd.h>

#define attr_num 100

float add_baseline(const float *a, const float *b){
	float c[attr_num] = {0.0};
	for (int i  = 0; i < attr_num; i++){
		c[i] = a[i] + b[i];
	}
	return c[attr_num - 1];
}

float addmul_baseline(const float *a, const float *b){
	float c[attr_num] = {0.0};
	for (int i  = 0; i < attr_num; i++){
		c[i] = a[i] * (i * 1.0) + b[i];
	}
	return c[attr_num - 1];
}

float add_avx(const float *a, const float *b){
	__attribute__ ((aligned (32))) float c[attr_num] = {0.0};
	for (int i = 0; i < attr_num / 8; i ++){
		__m256 x = _mm256_load_ps(a + 8 * i);
		__m256 y = _mm256_load_ps(b + 8 * i);
		__m256 ret = _mm256_add_ps(x, y);
		_mm256_store_ps(c + 8 * i, ret);
	}
	for (int i = attr_num - attr_num % 8; i < attr_num; i ++){
		c[i] = a[i] + b[i];
	} 
	return c[attr_num -1];
}

float addmul_avx(const float *a, const float *b){
	__attribute__ ((aligned (32))) float c[attr_num] = {0.0};
	for (int i = 0; i < attr_num / 8; i ++){
		__m256 x = _mm256_load_ps(a + 8 * i);
		__m256 y = _mm256_set_ps(7.0+i*8, 6.0+i*8, 5.0+i*8, 4.0+i*8, 3.0+i*8, 2.0+i*8, 1.0+i*8, 0.0+i*8);
		__m256 z = _mm256_mul_ps(x, y); 
		__m256 t = _mm256_load_ps(b + 8 *i);
		__m256 ret =  _mm256_add_ps(z, t);
		_mm256_store_ps(c + 8 * i, ret);
	}
	for (int i = attr_num - attr_num % 8; i < attr_num; i ++){
		c[i] = a[i] * i + b[i];
	} 
	return c[attr_num -1];
}

float addmul_fma(const float *a, const float *b){
	__attribute__ ((aligned (32))) float c[attr_num] ={0.0};
	for (int i = 0 ; i < attr_num /8; i ++){
		__m256 x = _mm256_load_ps(a + 8 * i);
		__m256 y = _mm256_set_ps(7.0+i*8, 6.0+i*8, 5.0+i*8, 4.0+i*8, 3.0+i*8, 2.0+i*8, 1.0+i*8, 0.0+i*8);
		__m256 z = _mm256_load_ps(b + 8 *i);
		__m256 ret = _mm256_fmadd_ps(x, y, z); //x*y+z
		_mm256_store_ps(c + 8 * i, ret);
	}
	for (int i = attr_num - attr_num % 8; i < attr_num; i ++){
		c[i] = a[i] *i + b[i];
	} 
	return c[attr_num -1];
}

int main()
{
	__attribute__ ((aligned (32))) float a[attr_num] = {0.0};
	__attribute__ ((aligned (32))) float b[attr_num] = {0.0};
	float base_time = 0;
	float used_time = 0;

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
	
	gettimeofday(&start_time, NULL); 
	for (int i = 1; i <= 100000000; i ++){
		addmul_avx(a, b);
		//printf("addmul_avx result %lf\n", addmul_avx(a, b));
	}
	gettimeofday(&end_time, NULL);
	used_time = (end_time.tv_sec + end_time.tv_usec/1000000.0) - (start_time.tv_sec + start_time.tv_usec/1000000.0);
	printf("addmul_avx used time: %lf speedup %lf \n", used_time, base_time / used_time);
	
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
